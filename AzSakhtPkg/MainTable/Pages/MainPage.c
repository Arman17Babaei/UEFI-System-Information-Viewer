#include "../Pages.h"
#include <Library/UefiLib.h>

PageItem smbiosTable = {
    .name = L"SMBIOS Table Data",
    .value = L"",
    .moreInformation = L"SMBIOS Table Data",
    .page = &smbiosPage,
};

PageItem processrsCount = {
    .name = L"Processors Count",
    .value = L"",
    .moreInformation = L"Number of Processors",
    .page = &processorPage,
};

PageItem acpiTable = {
    .name = L"ACPI Table",
    .value = L"",
    .moreInformation = L"ACPI Table Data",
    .page = &acpiPage,
};

Page mainPage = {
    .name = L"Main Page",
    .itemCount = 3,
    .pageItems =
        {
            &processrsCount,
            &smbiosTable,
            &acpiTable,
        },
};