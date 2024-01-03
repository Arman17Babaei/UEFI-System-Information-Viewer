#include "PageState.h"
#include "Pages.h"

#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Uefi.h>

CHAR16 filterTerm[MAX_NAME_LEN];
CHAR16 searchTerm[MAX_NAME_LEN];

CHAR16 pageC[HEIGHT][WIDTH];
CHAR16 preActions[HEIGHT][WIDTH][MAX_ACTION_LEN];
CHAR16 postActions[HEIGHT][WIDTH][MAX_ACTION_LEN];

CHAR16 pageStr[HEIGHT * (WIDTH) * (1 + MAX_ACTION_LEN * 2)];

UINTN rowIndex[HEIGHT];
UINTN pageRows;

VOID StartSearchFont(CHAR16 *string) {
  // set bold font and highlight background in preactions
  // string[0] = L'\x1b';
  // string[1] = L'[';
  // string[2] = L'1';
  // string[3] = L'm';
  // string[4] = L'\x1b';
  // string[5] = L'[';
  // string[6] = L'4';
  // string[7] = L'0';
  // string[8] = L'm';
}

VOID ResetFont(CHAR16 *string) {
  // reset font and background in postactions
  // string[0] = L'\x1b';
  // string[1] = L'[';
  // string[2] = L'0';
  // string[3] = L'm';
  // string[4] = L'\x1b';
  // string[5] = L'[';
  // string[6] = L'4';
  // string[7] = L'9';
  // string[8] = L'm';
}

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
        } else {
          break;
        }
      }
      pageStr[pos++] = pageC[i][j];
      for (UINTN k = 0; k < MAX_ACTION_LEN; k++) {
        if (postActions[i][j][k] != L'\0') {
          pageStr[pos++] = postActions[i][j][k];
        } else {
          break;
        }
      }
    }
    pageStr[pos++] = L'\n';
  }
  pageStr[pos] = L'\0';

  gST->ConOut->OutputString(gST->ConOut, pageStr);
}

VOID DrawPage(Page *page) {
  ResetFont(preActions[0][0]);

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
    pageC[5][i] = L'─';
    pageC[HEIGHT - 1][i] = L'─';
  }

  pageC[0][0] = L'┌';
  pageC[0][WIDTH - 1] = L'┐';
  pageC[1][0] = pageC[1][WIDTH - 1] = L'│';
  pageC[2][0] = L'├';
  pageC[2][WIDTH - 1] = L'┤';
  pageC[3][0] = pageC[3][MAX_NAME_LEN] = L'│';
  pageC[3][2 * MAX_NAME_LEN] = pageC[3][WIDTH - 1] = L'│';
  pageC[4][0] = pageC[4][MAX_NAME_LEN] = L'│';
  pageC[4][2 * MAX_NAME_LEN] = pageC[4][WIDTH - 1] = L'│';
  pageC[5][0] = L'├';
  pageC[5][WIDTH - 1] = L'┤';
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
    pageC[3][MAX_NAME_LEN * 2 + 2 + i] = f[i];
  }
  len = StrLen(filterTerm);
  for (int i = 0; i < len; i++) {
    pageC[3][MAX_NAME_LEN * 2 + 9 + i] = filterTerm[i];
  }
  if (position.onHeader && position.headerIndex == 1) {
    pageC[3][MAX_NAME_LEN * 2 + 1] = L'*';
  }

  CHAR16 s[] = L"Search: ";
  len = StrLen(s);
  for (int i = 0; i < len; i++) {
    pageC[4][MAX_NAME_LEN * 2 + 2 + i] = s[i];
  }
  len = StrLen(searchTerm);
  for (int i = 0; i < len; i++) {
    pageC[4][MAX_NAME_LEN * 2 + 9 + i] = searchTerm[i];
  }
  if (position.onHeader && position.headerIndex == 2) {
    pageC[4][MAX_NAME_LEN * 2 + 1] = L'*';
  }

  CHAR16 n[] = L"Name";
  len = StrLen(n);
  for (int i = 0; i < len; i++) {
    pageC[4][MAX_NAME_LEN / 2 - len / 2 + i] = n[i];
  }
  CHAR16 v[] = L"Value";
  len = StrLen(v);
  for (int i = 0; i < len; i++) {
    pageC[4][MAX_NAME_LEN + MAX_NAME_LEN / 2 - len / 2 + i] = v[i];
  }

  // Description
  CHAR16 description[MAX_DESCRIPTION_LEN];
  if (position.onHeader && position.headerIndex == 0) {
    StrCpyS(description, MAX_DESCRIPTION_LEN,
            L"Look around using up and down arrow keys. Press enter to select "
            L"an item. Press escape to go back.");
  } else {
    StrnCpyS(description, MAX_DESCRIPTION_LEN,
             page->pageItems[rowIndex[position.rowNumber]]->moreInformation,
             MAX_DESCRIPTION_LEN);
  }
  len = StrLen(description);
  int curX = MAX_NAME_LEN * 2 + 2;
  int curY = 6;
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

  // Filter Items
  pageRows = 0;
  for (int i = 0; i < page->itemCount; i++) {
    if (StrLen(filterTerm) == 0 ||
        MatchIndex(page->pageItems[i]->name, filterTerm) != -1) {
      rowIndex[pageRows++] = i;
    }
  }

  // Items
  CHAR16 *value;
  for (int i = 0; i < pageRows; i++) {
    int row = 6 + i;
    if (!position.onHeader && position.rowNumber == i) {
      pageC[row][0] = L'├';
      pageC[row][1] = L'*';
      pageC[row][MAX_NAME_LEN - 1] = L'*';
      pageC[row][MAX_NAME_LEN] = L'┤';
    }

    PageItem *item = page->pageItems[rowIndex[i]];
    len = StrLen(item->name);
    for (int j = 0; j < len; j++) {
      pageC[row][MAX_NAME_LEN / 2 - len / 2 + j] = item->name[j];
    }
    value = item->value;
    if (value == NULL) {
      value = L"";
    }
    len = StrLen(value);
    for (int j = 0; j < len; j++) {
      pageC[row][MAX_NAME_LEN + MAX_NAME_LEN / 2 - len / 2 + j] = value[j];
    }

    // Highlight search
    if (StrLen(searchTerm) != 0) {
      INT32 match = MatchIndex(item->name, searchTerm);
      if (match != -1) {
        len = StrLen(item->name);
        INT32 matchColumn = MAX_NAME_LEN / 2 - len / 2 + match;

        StartSearchFont(preActions[row][matchColumn]);
        ResetFont(postActions[row][matchColumn + StrLen(searchTerm) - 1]);
      }
    }
  }
}

VOID UpdateScreen() {
  ClearScreen();
  if (currentPage->Filler != NULL) {
    currentPage->Filler();
  }
  DrawPage(currentPage);
  PrintPage();
}

VOID HandleKeyStroke(EFI_INPUT_KEY key) {
  if (position.onHeader && position.headerIndex > 0) {
    if (key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
      position.onHeader = 0;
      position.rowNumber = 0;
      return;
    }
    if (position.headerIndex == 1) {
      UpdateTerm(key, filterTerm);
    } else if (position.headerIndex == 2) {
      UpdateTerm(key, searchTerm);
    }
  }
  if (key.ScanCode == SCAN_UP) {
    if (position.onHeader) {
      position.onHeader = 0;
      position.rowNumber = pageRows - 1;
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
      if (position.rowNumber >= pageRows) {
        position.onHeader = 1;
        position.headerIndex = 0;
      }
    }
  }
  if (key.ScanCode == SCAN_RIGHT) {
    if (!position.onHeader) {
      position.onHeader = 1;
      position.headerIndex = 1;
    } else {
      position.headerIndex++;
      if (position.headerIndex > 2) {
        position.headerIndex = 0;
      }
    }
  }
  if (key.ScanCode == SCAN_LEFT) {
    if (!position.onHeader) {
      position.onHeader = 1;
      position.headerIndex = 0;
    } else {
      if (position.headerIndex == 0) {
        position.headerIndex = 2;
      } else {
        position.headerIndex--;
      }
    }
  }
  if (key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
    if (!position.onHeader) {
      position.onHeader = 1;
      position.headerIndex = 0;
      PageItem *pageItem = currentPage->pageItems[rowIndex[position.rowNumber]];
      if (pageItem->page != NULL) {
        currentPage = pageItem->page;
      }
    }
  }
  if (key.ScanCode == SCAN_ESC) {
    currentPage = &mainPage;
    position.onHeader = 1;
    position.headerIndex = 0;
  }
}
