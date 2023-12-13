#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Uefi.h>

#include "Pages.h"
#include "PageState.h"

UINTN WIDTH = 300;
UINTN HEIGHT = 200;
Page *currentPage = &processorPage;

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE ImageHandle,
                           EFI_SYSTEM_TABLE *SystemTable) {
  EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *SimpleTextOut;
  SystemTable->BootServices->LocateProtocol(&gEfiSimpleTextOutProtocolGuid,
                                            NULL, (VOID **)&SimpleTextOut);

  if (SimpleTextOut != NULL) {
    SimpleTextOut->QueryMode(SimpleTextOut, 0, &WIDTH, &HEIGHT);   // Character width per line
  }
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
