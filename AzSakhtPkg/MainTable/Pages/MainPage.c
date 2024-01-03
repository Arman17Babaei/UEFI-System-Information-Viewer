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

INT32 SmbiosTableDataDescription(CHAR16 *res) {
  StrCpyS(res, MAX_NAME_LEN, L"SMBIOS Table Data");
  return 0;
}

INT32 AcpiTableDataDescription(CHAR16 *res) {
  StrCpyS(res, MAX_NAME_LEN, L"ACPI Table Data");
  return 0;
}

PageItem smbiosTable = {
    .name = L"SMBIOS Table Data",
    .GetValue = NoneString,
    .GetMoreInformation = SmbiosTableDataDescription,
    .page = &smbiosPage,
};

PageItem processrsCount = {
    .name = L"Processors Count",
    .GetValue = NoneString,
    .GetMoreInformation = ProcessorsCountDescription,
    .page = &processorPage,
};

PageItem acpiTable = {
    .name = L"ACPI Table",
    .GetValue = NoneString,
    .GetMoreInformation = AcpiTableDataDescription,
    .page = &acpiPage,
};

PageItem page2 = {
    .name = L"test value 2",
    .GetValue = TestString,
    .GetMoreInformation = TestString,
    .page = &mainPage,
};

Page mainPage = {
    .name = L"Main Page",
    .itemCount = 4,
    .pageItems =
        {
            &processrsCount,
            &smbiosTable,
            &acpiTable,
            &page2,
        },
};