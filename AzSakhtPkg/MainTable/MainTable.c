#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Uefi.h>

#include "MainPage.h"
#include "PageState.h"

Page* currentPage = &mainPage;

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE ImageHandle,
                           EFI_SYSTEM_TABLE *SystemTable) {
  UpdateScreen();
  EFI_INPUT_KEY Key;

  while (1) {
    SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
    if (Key.ScanCode + Key.UnicodeChar) {
      UpdateScreen();
      Print(L"Scan code: %d, UnicodeChar: %d\n", Key.ScanCode, Key.UnicodeChar);
    }
  }

  return EFI_SUCCESS;
}
