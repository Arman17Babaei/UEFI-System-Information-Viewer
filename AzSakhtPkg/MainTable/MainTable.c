#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#define NUM_ROWS 3
#define NUM_COLS 4
#define MAX_INPUT_LENGTH 50

CHAR16 *TableData[NUM_ROWS][NUM_COLS] = {
    { L"Row 1, Col 1", L"Row 1, Col 2", L"Row 1, Col 3", L"Row 1, Col 4" },
    { L"Row 2, Col 1", L"Row 2, Col 2", L"Row 2, Col 3", L"Row 2, Col 4" },
    { L"Row 3, Col 1", L"Row 3, Col 2", L"Row 3, Col 3", L"Row 3, Col 4" }
};

UINTN CurrentRow = 0;
UINTN CurrentCol = 0;

VOID ClearScreen() {
    gST->ConOut->ClearScreen(gST->ConOut);
}

VOID DisplayTable() {
    UINTN Row, Col;

    ClearScreen();
    
    Print(L"| Column 1 | Column 2 | Column 3 | Column 4 |\n");
    
    for (Row = 0; Row < NUM_ROWS; Row++) {
        Print(L"| ");
        for (Col = 0; Col < NUM_COLS; Col++) {
            if (Row == CurrentRow && Col == CurrentCol) {
                Print(L"\033[7m%-10s\033[0m | ", TableData[Row][Col]);
            } else {
                Print(L"%-10s | ", TableData[Row][Col]);
            }
        }
        Print(L"\n");
    }

    // Display the button in the middle of the last row
    if (CurrentRow == NUM_ROWS - 1) {
        Print(L"\n");
        Print(L"   [ Reset ]   "); // Placeholder for the button
    }
}

EFI_STATUS EFIAPI UefiMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    DisplayTable();

    EFI_INPUT_KEY Key;
    BOOLEAN Editing = FALSE;
    CHAR16 InputBuffer[MAX_INPUT_LENGTH];
    UINTN InputIndex = 0;

    while (1) {
        SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);

        if (Editing) {
            if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
                Editing = FALSE;
                InputBuffer[InputIndex] = L'\0';
                StrCpyS(TableData[CurrentRow][CurrentCol], 1024, InputBuffer);
                DisplayTable();
            } else if (Key.UnicodeChar == CHAR_BACKSPACE && InputIndex > 0) {
                InputIndex--;
                InputBuffer[InputIndex] = L'\0';
                DisplayTable();
            } else if (Key.UnicodeChar >= L' ' && InputIndex < MAX_INPUT_LENGTH - 1) {
                InputBuffer[InputIndex] = Key.UnicodeChar;
                InputIndex++;
                InputBuffer[InputIndex] = L'\0';
                DisplayTable();
            }
        } else {
            switch (Key.ScanCode) {
                case SCAN_UP:
                    if (CurrentRow > 0) {
                        CurrentRow--;
                        DisplayTable();
                    }
                    break;
                case SCAN_DOWN:
                    if (CurrentRow < NUM_ROWS - 1) {
                        CurrentRow++;
                        DisplayTable();
                    }
                    break;
                case SCAN_LEFT:
                    if (CurrentCol > 0) {
                        CurrentCol--;
                        DisplayTable();
                    }
                    break;
                case SCAN_RIGHT:
                    if (CurrentCol < NUM_COLS - 1) {
                        CurrentCol++;
                        DisplayTable();
                    }
                    break;
                case SCAN_F1:
                    Editing = TRUE;
                    Print(L"Editing");
                    gBS->Stall(1000000);
                    InputIndex = StrLen(TableData[CurrentRow][CurrentCol]);
                    StrCpyS(InputBuffer, 1024, TableData[CurrentRow][CurrentCol]);
                    DisplayTable();
                    break;
                case SCAN_ESC: // Reset values in the middle row
                    if (CurrentRow == NUM_ROWS - 1) {
                        for (UINTN Col = 0; Col < NUM_COLS; Col++) {
                            TableData[NUM_ROWS / 2][Col] = L"";
                        }
                        DisplayTable();
                    }
                    break;
                default:
                    if (Key.ScanCode != 0) {
                        Print(L"Scan code: %d\n", Key.ScanCode);
                    }
                    break;
            }
        }
    }

    return EFI_SUCCESS;
}
