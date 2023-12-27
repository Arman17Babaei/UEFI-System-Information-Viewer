#include "../Pages.h"
#include "../Utils.h"
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/MmMp.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>



#define CHECK_UPDATE                                                           \
  UpdateSmbiosProtocol();                                                         \
  if (SmbiosProtocol == NULL) {                                                     \
    return -1;                                                                 \
  }

EFI_SMBIOS_PROTOCOL *SmbiosProtocol = NULL;

CHAR8* GetRecordString(EFI_SMBIOS_TABLE_HEADER* Record, UINTN number){
  if (!number)
    return "";

  CHAR8* String = (CHAR8*)Record + Record->Length;
  UINTN i=1;
  while (i < number) {
    String = String + AsciiStrSize(String);
    i++;
  }
  return String;
}

VOID EFIAPI UpdateSmbiosProtocol() {
  if (SmbiosProtocol != NULL) {
    return;
  }

  EFI_STATUS Status = gBS->LocateProtocol(&gEfiSmbiosProtocolGuid, NULL, 
                                          (VOID**)&SmbiosProtocol);
  if (EFI_ERROR(Status)) {
    Print(L"Error locating SMBIOS protocol: %r\n", Status);
    return;
  }
}

INT32 GetBiosVersion(CHAR16 *res) {
    CHECK_UPDATE
    EFI_STATUS Status;
    EFI_SMBIOS_HANDLE SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    EFI_SMBIOS_TABLE_HEADER* Record;
    Status = SmbiosProtocol->GetNext(SmbiosProtocol, &SmbiosHandle, NULL, &Record, NULL);
    while (!EFI_ERROR(Status)) {
        switch (Record->Type) {
            case EFI_SMBIOS_TYPE_BIOS_INFORMATION: {
                SMBIOS_TABLE_TYPE0* Type0Record = (SMBIOS_TABLE_TYPE0*) Record;
                Int2Str(Type0Record->BiosVersion, res);
                //strcpy(res, GetRecordString(Record, Type0Record->BiosVersion));
                // res = Type0Record->BiosVersion
                Print(L"\tBiosVersion=%a\n", GetRecordString(Record, Type0Record->BiosVersion));
                Print(L"\tBiosReleaseDate=%a\n", GetRecordString(Record, Type0Record->BiosReleaseDate));
                break;
            }
        }
        Status = SmbiosProtocol->GetNext(SmbiosProtocol, &SmbiosHandle, NULL, &Record, NULL);
    }
    return 0;
}

INT32 GetBiosVersionDescription(CHAR16 *res) {
  StrCpyS(res, MAX_NAME_LEN, L"Version number or identifier of the system's BIOS.");
  return 0;
}

INT32 GetManufacturer(CHAR16 *res) {
    CHECK_UPDATE
    EFI_STATUS Status;
    EFI_SMBIOS_HANDLE SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    EFI_SMBIOS_TABLE_HEADER* Record;
    Status = SmbiosProtocol->GetNext(SmbiosProtocol, &SmbiosHandle, NULL, &Record, NULL);
    while (!EFI_ERROR(Status)) {
        switch (Record->Type) {
            case EFI_SMBIOS_TYPE_SYSTEM_INFORMATION: {
                SMBIOS_TABLE_TYPE1* Type1Record = (SMBIOS_TABLE_TYPE1*) Record;
                Int2Str(Type1Record->Manufacturer, res);
                // res = GetRecordString(Record, Type1Record->Manufacturer);
                break;
            }
        }
        Status = SmbiosProtocol->GetNext(SmbiosProtocol, &SmbiosHandle, NULL, &Record, NULL);
    }
    return 0;
}

INT32 GetManufacturerDescription(CHAR16 *res) {
  StrCpyS(res, MAX_NAME_LEN, L"Manufacturer of the system");
  return 0;
}

PageItem biosVersion = {
    .name = L"BIOS Version",
    .GetValue = GetBiosVersion,
    .GetMoreInformation = GetBiosVersionDescription,
    .page = NULL,
};

PageItem Manufacturer = {
    .name = L"Manufacturer",
    .GetValue = GetManufacturer,
    .GetMoreInformation = GetManufacturerDescription,
    .page = NULL,
};

Page smbiosPage = {
    .name = L"System Management BIOS Page",
    .itemCount = 2,
    .pageItems =
        {
            &biosVersion,
            &Manufacturer
        },
};