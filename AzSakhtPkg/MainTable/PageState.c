#include "PageState.h"

#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Uefi.h>

VOID ClearScreen() { gST->ConOut->ClearScreen(gST->ConOut); }

VOID PrintRow(CHAR16 *row, CHAR16 **preActions, CHAR16 **postActions) {
  for (int i = 0; i < WIDTH; i++) {
    if (preActions[i])
      Print(L"%s", preActions[i]);
    Print(L"%c", row[i]);
    if (postActions[i])
      Print(L"%s", postActions[i]);
  }
  Print(L"\n");
}

VOID DisplayPage(Page *page) {
  // HEADER
  CHAR16 row[WIDTH];
  CHAR16 *preActions[WIDTH];
  CHAR16 *postActions[WIDTH];
  SetMem(preActions, sizeof(preActions), 0);
  SetMem(postActions, sizeof(postActions), 0);

  for (int i = 0; i < WIDTH; i++) {
    row[i] = L'─';
  }
  row[0] = L'┌';
  row[WIDTH - 1] = L'┐';
  PrintRow(row, preActions, postActions);

  for (int i = 0; i < WIDTH; i++) {
    row[i] = L' ';
  }
  INT32 len = StrLen(page->name);
  for (int i = 0; i < len; i++) {
    row[WIDTH / 2 - len / 2 + i] = page->name[i];
  }
  row[0] = row[WIDTH - 1] = L'│';
  PrintRow(row, preActions, postActions);

  for (int i = 0; i < WIDTH; i++) {
    row[i] = L'─';
  }
  row[0] = L'├';
  row[WIDTH - 1] = L'┤';
  PrintRow(row, preActions, postActions);

  for (int i = 0; i < WIDTH; i++) {
    row[i] = L' ';
  }
  row[0] = row[MAX_NAME_LEN] = row[2 * MAX_NAME_LEN] = row[WIDTH - 1] = L'│';
  CHAR16 n[] = L"Name";
  len = StrLen(n);
  for (int i = 0; i < len; i++) {
    row[MAX_NAME_LEN / 2 - len / 2 + i] = n[i];
  }
  CHAR16 v[] = L"Value";
  len = StrLen(v);
  for (int i = 0; i < len; i++) {
    row[MAX_NAME_LEN + MAX_NAME_LEN / 2 - len / 2 + i] = v[i];
  }
  PrintRow(row, preActions, postActions);

  for (int i = 0; i < WIDTH; i++) {
    row[i] = L'─';
  }
  row[0] = L'├';
  row[WIDTH - 1] = L'┤';
  PrintRow(row, preActions, postActions);

  CHAR16 value[MAX_NAME_LEN];
  for (int i = 0; i < page->itemCount; i++) {
    for (int j = 0; j < WIDTH; j++) {
      row[j] = L' ';
    }
    row[0] = row[MAX_NAME_LEN] = row[2 * MAX_NAME_LEN] = row[WIDTH - 1] = L'│';

    PageItem *item = page->pageItems[i];
    len = StrLen(item->name);
    for (int i = 0; i < len; i++) {
      row[MAX_NAME_LEN / 2 - len / 2 + i] = item->name[i];
    }
    item->GetValue(value);
    len = StrLen(value);
    for (int i = 0; i < len; i++) {
      row[MAX_NAME_LEN + MAX_NAME_LEN / 2 - len / 2 + i] = value[i];
    }
    PrintRow(row, preActions, postActions);
  }

  for (int i = 0; i < WIDTH; i++) {
    row[i] = L'─';
  }
  row[0] = L'└';
  row[WIDTH - 1] = L'┘';
  PrintRow(row, preActions, postActions);
}

VOID UpdateScreen() {
  ClearScreen();
  DisplayPage(currentPage);
}