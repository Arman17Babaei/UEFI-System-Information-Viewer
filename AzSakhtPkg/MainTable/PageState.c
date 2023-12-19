#include "PageState.h"
#include "Pages.h"

#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Uefi.h>

#define MAX_ACTION_LEN 5
#define MAX_DESCRIPTION_LEN 500

CHAR16 filterTerm[MAX_NAME_LEN];

CHAR16 pageC[HEIGHT][WIDTH];
CHAR16 preActions[HEIGHT][WIDTH][MAX_ACTION_LEN];
CHAR16 postActions[HEIGHT][WIDTH][MAX_ACTION_LEN];

CHAR16 pageStr[HEIGHT * (WIDTH) * (1 + MAX_ACTION_LEN * 2)];

VOID ClearScreen() {
  for (UINTN i = 0; i < HEIGHT; i++) {
    for (UINTN j = 0; j < WIDTH; j++) {
      pageC[i][j] = L' ';
      for (UINTN k = 0; k < MAX_ACTION_LEN; k++) {
        preActions[i][j][k] = L'\0';
        postActions[i][j][k] = L'\0';
      }
    }
  }
  gST->ConOut->ClearScreen(gST->ConOut);
}

VOID PrintPage() {
  UINTN pos = 0;
  for (UINTN i = 0; i < HEIGHT; i++) {
    for (UINTN j = 0; j < WIDTH; j++) {
      for (UINTN k = 0; k < MAX_ACTION_LEN; k++) {
        if (preActions[i][j][k] != L'\0') {
          pageStr[pos++] = preActions[i][j][k];
        }
      }
      pageStr[pos++] = pageC[i][j];
      for (UINTN k = 0; k < MAX_ACTION_LEN; k++) {
        if (postActions[i][j][k] != L'\0') {
          pageStr[pos++] = postActions[i][j][k];
        }
      }
    }
    pageStr[pos++] = L'\n';
  }
  pageStr[pos] = L'\0';

  gST->ConOut->OutputString(gST->ConOut, pageStr);
}

VOID DrawPage(Page *page) {
  // Frame
  for (UINTN i = 0; i < HEIGHT; i++) {
    pageC[i][0] = L'│';
    if (i > 4) {
      pageC[i][MAX_NAME_LEN] = L'│';
      pageC[i][2 * MAX_NAME_LEN] = L'│';
    }
    pageC[i][WIDTH - 1] = L'│';
  }
  for (int i = 0; i < WIDTH; i++) {
    pageC[0][i] = L'─';
    pageC[2][i] = L'─';
    pageC[4][i] = L'─';
    pageC[HEIGHT - 1][i] = L'─';
  }

  pageC[0][0] = L'┌';
  pageC[0][WIDTH - 1] = L'┐';
  pageC[1][0] = pageC[1][WIDTH - 1] = L'│';
  pageC[2][0] = L'├';
  pageC[2][WIDTH - 1] = L'┤';
  pageC[3][0] = pageC[3][MAX_NAME_LEN] = L'│';
  pageC[3][2 * MAX_NAME_LEN] = pageC[3][WIDTH - 1] = L'│';
  pageC[4][0] = L'├';
  pageC[4][WIDTH - 1] = L'┤';
  pageC[HEIGHT - 1][0] = L'└';
  pageC[HEIGHT - 1][WIDTH - 1] = L'┘';

  // Page title
  INT32 len = StrLen(page->name);
  for (int i = 0; i < len; i++) {
    pageC[1][WIDTH / 2 - len / 2 + i] = page->name[i];
  }
  if (position.onHeader && position.headerIndex == 0) {
    pageC[1][WIDTH / 2 - len / 2 - 1] = L'├';
    pageC[1][WIDTH / 2 - len / 2 + len] = L'┤';
  }

  CHAR16 f[] = L"Filter: ";
  len = StrLen(f);
  for (int i = 0; i < len; i++) {
    pageC[3][MAX_NAME_LEN * 2 + 1 + i] = f[i];
  }
  len = StrLen(filterTerm);
  for (int i = 0; i < len; i++) {
    pageC[3][MAX_NAME_LEN * 2 + 8 + i] = filterTerm[i];
  }

  CHAR16 n[] = L"Name";
  len = StrLen(n);
  for (int i = 0; i < len; i++) {
    pageC[3][MAX_NAME_LEN / 2 - len / 2 + i] = n[i];
  }
  CHAR16 v[] = L"Value";
  len = StrLen(v);
  for (int i = 0; i < len; i++) {
    pageC[3][MAX_NAME_LEN + MAX_NAME_LEN / 2 - len / 2 + i] = v[i];
  }

  // Description
  CHAR16 description[MAX_DESCRIPTION_LEN];
  if (position.onHeader && position.headerIndex == 0) {
    StrCpyS(description, MAX_DESCRIPTION_LEN,
            L"Look around using up and down arrow keys. Press enter to select "
            L"an item. Press escape to go back.");
  } else {
    page->pageItems[position.rowNumber]->GetMoreInformation(description);
  }
  len = StrLen(description);
  int curX = MAX_NAME_LEN * 2 + 2;
  int curY = 5;
  for (int i = 0; i < len; i++) {
    if (curX >= WIDTH - 1) {
      curX = MAX_NAME_LEN * 2 + 2;
      curY++;
    }
    if (description[i] == L'\n') {
      curX = MAX_NAME_LEN * 2 + 2;
      curY++;
      continue;
    }
    pageC[curY][curX++] = description[i];
  }

  // Items
  CHAR16 value[MAX_NAME_LEN];
  int p = 0;
  for (int i = 0; i < page->itemCount; i++) {
    if (StrLen(filterTerm) > 0) {
      if (StrLen(filterTerm) > StrLen(page->pageItems[i]->name)) {
        continue;
      }
      int match = 0;
      for (int k = 0;
           k < StrLen(page->pageItems[i]->name) - StrLen(filterTerm) + 1; k++) {
        int equal = 1;
        for (int j = 0; j < StrLen(filterTerm); j++) {
          if (filterTerm[j] != page->pageItems[i]->name[k + j]) {
            equal = 0;
          }
        }
        if (equal)
          match = 1;
      }
      if (!match) {
        continue;
      }
    }
    int row = 5 + p;
    if (!position.onHeader && position.rowNumber == i) {
      pageC[row][0] = L'├';
      pageC[row][1] = L'*';
      pageC[row][MAX_NAME_LEN - 1] = L'*';
      pageC[row][MAX_NAME_LEN] = L'┤';
    }

    PageItem *item = page->pageItems[i];
    len = StrLen(item->name);
    for (int j = 0; j < len; j++) {
      pageC[row][MAX_NAME_LEN / 2 - len / 2 + j] = item->name[j];
    }
    item->GetValue(value);
    len = StrLen(value);
    for (int j = 0; j < len; j++) {
      pageC[row][MAX_NAME_LEN + MAX_NAME_LEN / 2 - len / 2 + j] = value[j];
    }
    p ++;
  }
}

VOID UpdateScreen() {
  ClearScreen();
  DrawPage(currentPage);
  PrintPage();
}

VOID HandleKeyStroke(EFI_INPUT_KEY key) {
  if (position.onHeader && position.headerIndex == 1) {
    if (key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
      position.onHeader = 0;
      position.rowNumber = 0;
      return;
    }
    if (key.UnicodeChar != 0) {
      if (StrLen(filterTerm) >= MAX_NAME_LEN - 1) {
        return;
      }
      filterTerm[StrLen(filterTerm) + 1] = L'\0';
      filterTerm[StrLen(filterTerm)] = key.UnicodeChar;
    }
    else {
      if (key.ScanCode == CHAR_BACKSPACE) {
        if (StrLen(filterTerm) == 0) {
          return;
        }
        filterTerm[StrLen(filterTerm) - 1] = L'\0';
      }
    }
  }
  if (key.ScanCode == SCAN_UP) {
    if (position.onHeader) {
      position.onHeader = 0;
      position.rowNumber = currentPage->itemCount - 1;
    } else {
      if (position.rowNumber == 0) {
        position.onHeader = 1;
        position.headerIndex = 0;
      }
      position.rowNumber--;
    }
  }
  if (key.ScanCode == SCAN_DOWN) {
    if (position.onHeader) {
      position.onHeader = 0;
      position.rowNumber = 0;
    } else {
      position.rowNumber++;
      if (position.rowNumber >= currentPage->itemCount) {
        position.onHeader = 1;
        position.headerIndex = 0;
      }
    }
  }
  if (key.ScanCode == SCAN_RIGHT) {
    position.onHeader = 1;
    position.headerIndex = 1;
  }
  if (key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
    if (!position.onHeader) {
      position.onHeader = 1;
      position.headerIndex = 0;
      if (currentPage->pageItems[position.rowNumber]->page != NULL) {
        currentPage = currentPage->pageItems[position.rowNumber]->page;
      }
    }
  }
  if (key.ScanCode == SCAN_ESC) {
    currentPage = &mainPage;
    position.onHeader = 1;
    position.headerIndex = 0;
  }
}
