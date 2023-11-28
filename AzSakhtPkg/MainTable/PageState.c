#include "PageState.h"

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Uefi.h>

VOID ClearScreen() { gST->ConOut->ClearScreen(gST->ConOut); }

VOID DisplayPage(Page *page) {
  // HEADER
  Print(L"##%-20s %s %-20s##\n", L"", page->name, L"");

  Print(L"Key%-23s Value\n", L"");
  CHAR16 value[MAX_NAME_LEN];
  for (int i = 0; i < page->itemCount; i++) {
    PageItem *item = page->pageItems[i];
    item->GetValue(value);
    Print(L"%-25s %-25s\n", item->name, value);
  }
}

VOID UpdateScreen() {
  ClearScreen();
  DisplayPage(currentPage);
}