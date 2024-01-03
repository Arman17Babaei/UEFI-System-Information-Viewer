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
                AsciiStrToUnicodeStrS(GetRecordString(Record, Type0Record->BiosVersion), res, MAX_NAME_LEN);
                break;
            }
        }
        Status = SmbiosProtocol->GetNext(SmbiosProtocol, &SmbiosHandle, NULL, &Record, NULL);
    }
    return 0;
}

INT32 GetBiosVersionDescription(CHAR16 *res) {
  StrCpyS(res, MAX_DESCRIPTION_LEN, L"Version number or identifier of the system's BIOS.");
  return 0;
}

INT32 GetBiosReleaseDate(CHAR16 *res) {
    CHECK_UPDATE
    EFI_STATUS Status;
    EFI_SMBIOS_HANDLE SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    EFI_SMBIOS_TABLE_HEADER* Record;
    Status = SmbiosProtocol->GetNext(SmbiosProtocol, &SmbiosHandle, NULL, &Record, NULL);
    while (!EFI_ERROR(Status)) {
        switch (Record->Type) {
            case EFI_SMBIOS_TYPE_BIOS_INFORMATION: {
                SMBIOS_TABLE_TYPE0* Type0Record = (SMBIOS_TABLE_TYPE0*) Record;
                AsciiStrToUnicodeStrS(GetRecordString(Record, Type0Record->BiosReleaseDate), res, MAX_NAME_LEN);
                break;
            }
        }
        Status = SmbiosProtocol->GetNext(SmbiosProtocol, &SmbiosHandle, NULL, &Record, NULL);
    }
    return 0;
}

INT32 GetBiosReleaseDateDescription(CHAR16 *res) {
  StrCpyS(res, MAX_DESCRIPTION_LEN, L"Release date of the system's BIOS.");
  return 0;
}

INT32 GetManufacturer(CHAR16 *res) {
    CHECK_UPDATE
    EFI_STATUS Status;
    EFI_SMBIOS_HANDLE SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    EFI_SMBIOS_TABLE_HEADER* Record;
    Status = SmbiosProtocol->GetNext(SmbiosProtocol, &SmbiosHandle, NULL, &Record, NULL);
    res[0] = 0;
    while (!EFI_ERROR(Status)) {
        switch (Record->Type) {
            case EFI_SMBIOS_TYPE_SYSTEM_INFORMATION: {
                SMBIOS_TABLE_TYPE1* Type1Record = (SMBIOS_TABLE_TYPE1*) Record;
                AsciiStrToUnicodeStrS(GetRecordString(Record, Type1Record->Manufacturer), res, MAX_NAME_LEN);
                break;
            }
        }
        Status = SmbiosProtocol->GetNext(SmbiosProtocol, &SmbiosHandle, NULL, &Record, NULL);
    }
    return 0;
}

INT32 GetManufacturerDescription(CHAR16 *res) {
  StrCpyS(res, MAX_DESCRIPTION_LEN, L"Manufacturer of the system");
  return 0;
}

INT32 GetProductName(CHAR16 *res) {
    CHECK_UPDATE
    EFI_STATUS Status;
    EFI_SMBIOS_HANDLE SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    EFI_SMBIOS_TABLE_HEADER* Record;
    Status = SmbiosProtocol->GetNext(SmbiosProtocol, &SmbiosHandle, NULL, &Record, NULL);
    while (!EFI_ERROR(Status)) {
        switch (Record->Type) {
            case EFI_SMBIOS_TYPE_SYSTEM_INFORMATION: {
                SMBIOS_TABLE_TYPE1* Type1Record = (SMBIOS_TABLE_TYPE1*) Record;
                AsciiStrToUnicodeStrS(GetRecordString(Record, Type1Record->ProductName), res, MAX_NAME_LEN);
                break;
            }
        }
        Status = SmbiosProtocol->GetNext(SmbiosProtocol, &SmbiosHandle, NULL, &Record, NULL);
    }
    return 0;
}

INT32 GetProductNameDescription(CHAR16 *res) {
  StrCpyS(res, MAX_DESCRIPTION_LEN, L"Product Name");
  return 0;
}

INT32 GetProcessorVersion(CHAR16 *res) {
    CHECK_UPDATE
    EFI_STATUS Status;
    EFI_SMBIOS_HANDLE SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    EFI_SMBIOS_TABLE_HEADER* Record;
    Status = SmbiosProtocol->GetNext(SmbiosProtocol, &SmbiosHandle, NULL, &Record, NULL);
    while (!EFI_ERROR(Status)) {
        switch (Record->Type) {
            case EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION: {
                SMBIOS_TABLE_TYPE4* Type4Record = (SMBIOS_TABLE_TYPE4*) Record;
                AsciiStrToUnicodeStrS(GetRecordString(Record, Type4Record->ProcessorVersion), res, MAX_NAME_LEN);    
                break;
            }
        }
        Status = SmbiosProtocol->GetNext(SmbiosProtocol, &SmbiosHandle, NULL, &Record, NULL);
    }
    return 0;
}

INT32 GetProcessorVersionDescription(CHAR16 *res) {
  StrCpyS(res, MAX_DESCRIPTION_LEN, L"Version of Processor");
  return 0;
}

INT32 GetProcessorManufacturer(CHAR16 *res) {
    CHECK_UPDATE
    EFI_STATUS Status;
    EFI_SMBIOS_HANDLE SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    EFI_SMBIOS_TABLE_HEADER* Record;
    Status = SmbiosProtocol->GetNext(SmbiosProtocol, &SmbiosHandle, NULL, &Record, NULL);
    while (!EFI_ERROR(Status)) {
        switch (Record->Type) {
            case EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION: {
                SMBIOS_TABLE_TYPE4* Type4Record = (SMBIOS_TABLE_TYPE4*) Record;
                AsciiStrToUnicodeStrS(GetRecordString(Record, Type4Record->ProcessorManufacturer), res, MAX_NAME_LEN);      
                break;
            }
        }
        Status = SmbiosProtocol->GetNext(SmbiosProtocol, &SmbiosHandle, NULL, &Record, NULL);
    }
    return 0;
}

INT32 GetProcessorManufacturerDescription(CHAR16 *res) {
  StrCpyS(res, MAX_DESCRIPTION_LEN, L"Manufacturer of Processor");
  return 0;
}

INT32 GetProcessorMaxSpeed(CHAR16 *res) {
    CHECK_UPDATE
    EFI_STATUS Status;
    EFI_SMBIOS_HANDLE SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
    EFI_SMBIOS_TABLE_HEADER* Record;
    Status = SmbiosProtocol->GetNext(SmbiosProtocol, &SmbiosHandle, NULL, &Record, NULL);
    while (!EFI_ERROR(Status)) {
        switch (Record->Type) {
            case EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION: {
                SMBIOS_TABLE_TYPE4* Type4Record = (SMBIOS_TABLE_TYPE4*) Record;
                Int2Str(Type4Record->MaxSpeed, res);
                // AsciiStrToUnicodeStrS(GetRecordString(Record, Type4Record->MaxSpeed), res, MAX_NAME_LEN);      
                break;
            }
        }
        Status = SmbiosProtocol->GetNext(SmbiosProtocol, &SmbiosHandle, NULL, &Record, NULL);
    }
    return 0;
}

INT32 GetMaxSpeedDescription(CHAR16 *res) {
  StrCpyS(res, MAX_DESCRIPTION_LEN, L"Maximum speed of the processor");
  return 0;
}

PageItem biosVersion = {
    .name = L"BIOS Version",
    .GetValue = GetBiosVersion,
    .GetMoreInformation = GetBiosVersionDescription,
    .page = NULL,
};

PageItem biosReleaseDate = {
    .name = L"BIOS Release Date",
    .GetValue = GetBiosReleaseDate,
    .GetMoreInformation = GetBiosReleaseDateDescription,
    .page = NULL,
};

PageItem manufacturer = {
    .name = L"Manufacturer",
    .GetValue = GetManufacturer,
    .GetMoreInformation = GetManufacturerDescription,
    .page = NULL,
};

PageItem productName = {
    .name = L"Product Name",
    .GetValue = GetProductName,
    .GetMoreInformation = GetProductNameDescription,
    .page = NULL,
};

PageItem processorVersion = {
    .name = L"Processor Version",
    .GetValue = GetProcessorVersion,
    .GetMoreInformation = GetProcessorVersionDescription,
    .page = NULL,
};

PageItem processorManufacturer = {
    .name = L"Processor Manufacturer",
    .GetValue = GetProcessorManufacturer,
    .GetMoreInformation = GetProcessorManufacturerDescription,
    .page = NULL,
};

PageItem processorMaxSpeed = {
    .name = L"Processor Max Speed",
    .GetValue = GetProcessorMaxSpeed,
    .GetMoreInformation = GetMaxSpeedDescription,
    .page = NULL,
};

Page smbiosPage = {
    .name = L"System Management BIOS Page",
    .itemCount = 7,
    .pageItems =
        {
            &biosVersion,
            &biosReleaseDate,
            &manufacturer,
            &productName,
            &processorVersion,
            &processorManufacturer,
            &processorMaxSpeed,
        },
};