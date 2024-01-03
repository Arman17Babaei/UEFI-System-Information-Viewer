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
    return;                                                                    \
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

PageItem signature = {
    .name = L"Signature",
    .moreInformation = L"Signature",
    .value = NULL,
    .page = NULL,
};

PageItem revision = {
    .name = L"Revision",
    .moreInformation = L"Revision",
    .value = NULL,
    .page = NULL,
};

PageItem creatorId = {
    .name = L"CreatorId",
    .moreInformation = L"CreatorId",
    .value = NULL,
    .page = NULL,
};

PageItem creatorRevision = {
    .name = L"CreatorRevision",
    .moreInformation = L"CreatorRevision",
    .value = NULL,
    .page = NULL,
};

PageItem items = {
    .name = L"Items",
    .moreInformation = L"Items",
    .value = L"...",
    .page = NULL,
};

VOID FillAcpiPage() {
  CHECK_UPDATE
  EFI_ACPI_6_3_ROOT_SYSTEM_DESCRIPTION_POINTER *RSDP = NULL;

  for (UINTN i = 0; i < gST->NumberOfTableEntries; i++) {
    if (CompareGuid(&(gST->ConfigurationTable[i].VendorGuid),
                    &gEfiAcpi20TableGuid)) {
      Print(L"RSDP table is placed at %p\n\n",
            gST->ConfigurationTable[i].VendorTable);

      RSDP = gST->ConfigurationTable[i].VendorTable;
      EFI_ACPI_DESCRIPTION_HEADER *XSDT =
          (EFI_ACPI_DESCRIPTION_HEADER *)RSDP->XsdtAddress;

      if (signature.value == NULL) {
        gBS->AllocatePool(EfiLoaderData, MAX_NAME_LEN * sizeof(CHAR16),
                          (VOID **)&signature.value);
      }
      Int2Str(XSDT->Signature, signature.value);

      if (revision.value == NULL) {
        gBS->AllocatePool(EfiLoaderData, MAX_NAME_LEN * sizeof(CHAR16),
                          (VOID **)&revision.value);
      }
      Int2Str(XSDT->Revision, revision.value);

      if (creatorId.value == NULL) {
        gBS->AllocatePool(EfiLoaderData, MAX_NAME_LEN * sizeof(CHAR16),
                          (VOID **)&creatorId.value);
      }
      Int2Str(XSDT->CreatorId, creatorId.value);

      if (creatorRevision.value == NULL) {
        gBS->AllocatePool(EfiLoaderData, MAX_NAME_LEN * sizeof(CHAR16),
                          (VOID **)&creatorRevision.value);
      }
      Int2Str(XSDT->CreatorRevision, creatorRevision.value);
    }
  }
}

Page acpiPage = {
    .name = L"ACPI Page",
    .itemCount = 5,
    .pageItems =
        {
            &signature,
            &revision,
            &creatorId,
            &creatorRevision,
            &items,
        },
    .Filler = FillAcpiPage,
};