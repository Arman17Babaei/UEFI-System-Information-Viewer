#include "Utils.h"

UINTN Int2Str(INT32 num, CHAR16 *res) {
  UINTN len = 0;
  while (num > 0) {
    res[len++] = num % 10 + '0';
    num /= 10;
  }
  res[len] = '\0';
  return len;
}

INT32 NoneString(CHAR16 *res) {
  res[0] = '\0';
  return 0;
}
