#include "../Pages.h"
#include "../Utils.h"
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/MmMp.h>
#include <Protocol/MpService.h>

#define CHECK_UPDATE                                                           \
  UpdateMpService();                                                           \
  if (MpService == NULL) {                                                     \
    return -1;                                                                 \
  }


EFI_MP_SERVICES_PROTOCOL *MpService = NULL;

VOID EFIAPI UpdateMpService() {
  if (MpService != NULL) {
    return;
  }

  EFI_STATUS Status = gBS->LocateProtocol(&gEfiMpServiceProtocolGuid, NULL,
                                          (VOID **)&MpService);
  if (EFI_ERROR(Status)) {
    Print(L"Failed to locate MP Service Protocol: %r\n", Status);
    return;
  }
}

INT32 GetNumberOfProcessors(CHAR16 *res) {
  CHECK_UPDATE

  UINTN NumberOfProcessors;
  UINTN NumberOfEnabledProcessors;
  MpService->GetNumberOfProcessors(MpService, &NumberOfProcessors,
                                   &NumberOfEnabledProcessors);
  Int2Str(NumberOfProcessors, res);
  return 0;
}

INT32 GetNumberOfProcessorsDescription(CHAR16 *res) {
  StrCpyS(res, MAX_NAME_LEN, L"Number of Processors");
  return 0;
}

INT32 GetNumberOfEnabledProcessors(CHAR16 *res) {
  CHECK_UPDATE

  UINTN NumberOfProcessors;
  UINTN NumberOfEnabledProcessors;
  MpService->GetNumberOfProcessors(MpService, &NumberOfProcessors,
                                   &NumberOfEnabledProcessors);
  Int2Str(NumberOfEnabledProcessors, res);
  return 0;
}

INT32 GetNumberOfEnabledProcessorsDescription(CHAR16 *res) {
  StrCpyS(res, MAX_NAME_LEN, L"Number of Enabled Processors");
  return 0;
}

PageItem numberOfProcessors = {
    .name = L"Number of Processors",
    .GetValue = GetNumberOfProcessors,
    .GetMoreInformation = GetNumberOfProcessorsDescription,
    .page = NULL,
};

PageItem numberOfEnabledProcessors = {
    .name = L"Number of Enabled Processors",
    .GetValue = GetNumberOfEnabledProcessors,
    .GetMoreInformation = GetNumberOfEnabledProcessorsDescription,
    .page = NULL,
};

Page processorPage = {
    .name = L"Processors Count Page",
    .itemCount = 2,
    .pageItems =
        {
            &numberOfProcessors,
            &numberOfEnabledProcessors,
        },
};
