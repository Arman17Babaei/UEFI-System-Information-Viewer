#ifndef TYPES_H_
#define TYPES_H_

#pragma pack(1) // Ensures byte alignment

#include <Uefi.h>

#define MAX_ITEMS_PER_PAGE 20
#define MAX_NAME_LEN 40

struct PageItem;

typedef struct {
  CHAR16 name[MAX_NAME_LEN];
  INT32 itemCount;
  struct PageItem *pageItems[100];
} Page;

typedef struct PageItem {
  CHAR16 name[MAX_NAME_LEN];
  INT32 (*GetValue)(CHAR16 *);
  INT32 (*GetMoreInformation)(CHAR16 *);
  Page *page;
} PageItem;

typedef struct {
  UINTN onHeader;    // Which of the two numbers below are valid
  UINTN headerIndex; // Main window, Search, Filter
  UINTN rowNumber;
} Position;

#endif // TYPES_H_
