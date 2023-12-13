#include "../Pages.h"
#include <Library/UefiLib.h>

INT32 TestString(CHAR16 *returnValue) {
  StrCpyS(returnValue, MAX_NAME_LEN, L"This is a test string!");
  return 0;
}

INT32 ProcessorsCountDescription(CHAR16 *res) {
  StrCpyS(res, MAX_NAME_LEN, L"Number of Processors");
  return 0;
}

PageItem processrsCount = {
    .name = L"Processors Count",
    .GetValue = NoneString,
    .GetMoreInformation = ProcessorsCountDescription,
    .page = &processorPage,
};

PageItem page2 = {
    .name = L"test value 2",
    .GetValue = TestString,
    .GetMoreInformation = TestString,
    .page = &mainPage,
};

Page mainPage = {
    .name = L"Main Page",
    .itemCount = 2,
    .pageItems =
        {
            &processrsCount,
            &page2,
        },
};
