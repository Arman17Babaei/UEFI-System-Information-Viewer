#include "MainPage.h"
#include <Library/UefiLib.h>

INT32 TestString(CHAR16 *returnValue) {
  StrCpyS(returnValue, 50, L"This is a test string!");
  return 0;
}

PageItem page1 = {
    .name = L"test value 1",
    .GetValue = TestString,
    .GetMoreInformation = TestString,
    .page = &mainPage,
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
            &page1,
            &page2,
        },
};
