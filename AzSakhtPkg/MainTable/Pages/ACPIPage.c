#include "../Pages.h"
#include "../Utils.h"
#include <IndustryStandard/Acpi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/Shell.h>
#include <Uefi.h>

#define CHECK_UPDATE                                                           \
  UpdateShellProtocol();                                                       \
  if (ShellProtocol == NULL) {                                                 \
    return -1;                                                                 \
  }

EFI_SHELL_PROTOCOL *ShellProtocol = NULL;

VOID EFIAPI UpdateShellProtocol() {
  if (ShellProtocol != NULL) {
    return;
  }

  EFI_STATUS Status = gBS->LocateProtocol(&gEfiShellProtocolGuid, NULL,
                                          (VOID **)&ShellProtocol);
  if (EFI_ERROR(Status)) {
    Print(L"Can't open EFI_SHELL_PROTOCOL: %r\n", Status);
    return;
  }
}

INT32 GetRsdpAddress(CHAR16 *res) {
  CHECK_UPDATE
  
  // gST->NumberOfTableEntries
  // EFI_ACPI_6_3_ROOT_SYSTEM_DESCRIPTION_POINTER* RSDP = NULL;

  // for (UINTN i=0; i<SystemTable->NumberOfTableEntries; i++) {
  //     if (CompareGuid(&(SystemTable->ConfigurationTable[i].VendorGuid),
  //     &gEfiAcpi20TableGuid)) {
  //         Print(L"RSDP table is placed at %p\n\n",
  //         SystemTable->ConfigurationTable[i].VendorTable); RSDP =
  //         SystemTable->ConfigurationTable[i].VendorTable;
  //     }
  // }

  // if (!RSDP) {
  //     Print(L"No ACPI2.0 table was found in the system\n");
  //     return 1;
  // }

  // if (((CHAR8)((RSDP->Signature >>  0) & 0xFF) != 'R') ||
  // ((CHAR8)((RSDP->Signature >>  8) & 0xFF) != 'S') ||
  //     ((CHAR8)((RSDP->Signature >> 16) & 0xFF) != 'D') ||
  //     ((CHAR8)((RSDP->Signature >> 24) & 0xFF) != ' ') ||
  //     ((CHAR8)((RSDP->Signature >> 32) & 0xFF) != 'P') ||
  //     ((CHAR8)((RSDP->Signature >> 40) & 0xFF) != 'T') ||
  //     ((CHAR8)((RSDP->Signature >> 48) & 0xFF) != 'R') ||
  //     ((CHAR8)((RSDP->Signature >> 56) & 0xFF) != ' ')) {

  //     Print(L"Error! RSDP signature is not valid!\n");
  //     return 1;
  // }
  // Print(L"\tRSDT table is placed at address %p\n", RSDP->RsdtAddress);
  return 0;
}

PageItem rsdpAddress = {
    .name = L"RSDP Table Placement",
    .value = NULL,
    .moreInformation = L"The address of RSDT table in the system",
    .page = NULL,
};

Page acpiPage = {
    .name = L"ACPI Page",
    .itemCount = 1,
    .pageItems =
        {
            &rsdpAddress,
            //&serialNumber,
        },
};