#ifndef UTILS_H_
#define UTILS_H_

#pragma pack(1) // Ensures byte alignment

#include <Uefi.h>

UINTN Int2Str(INT32 num, CHAR16 *res);

INT32 NoneString(CHAR16 *res);

#endif // UTILS_H_
