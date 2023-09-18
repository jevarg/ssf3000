#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <ScrnSave.h>
#include <strsafe.h>

#include "main.h"
#include "F3000.h"
#include "gfx/dx/DXRenderer.h"

LONG lSpeed = DEFVEL;                   // redraw speed variable
int i = 0;

CHAR szAppName[] = "Farming3000";
CHAR szTemp[20];                        // temporary array of characters
CHAR szRedrawSpeed[] = "Redraw Speed";  // .ini speed entry
CHAR szIniFile[MAXFILELEN];             // .ini or registry file name

F3000 *f3000;
DXRenderer *renderer;

BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND hSpeed;   // handle to speed scroll bar
    static HWND hOK;      // handle to OK push button

    switch (message) {
        case WM_INITDIALOG:

            // Retrieve the application name from the .rc file.
            LoadString(hMainInstance, idsAppName, szAppName,
                       80 * sizeof(TCHAR));

            // Retrieve the .ini (or registry) file name.
            LoadString(hMainInstance, idsIniFile, szIniFile,
                       MAXFILELEN * sizeof(TCHAR));

            // TODO: Add error checking to verify LoadString success
            //       for both calls.

            // Retrieve any redraw speed data from the registry.
            lSpeed = GetPrivateProfileInt(szAppName, szRedrawSpeed,
                                          DEFVEL, szIniFile);

            // If the initialization file does not contain an entry
            // for this screen saver, use the default value.
            if (lSpeed > MAXVEL || lSpeed < MINVEL)
                lSpeed = DEFVEL;

            // Initialize the redraw speed scroll bar control.
            hSpeed = GetDlgItem(hDlg, ID_SPEED);
            SetScrollRange(hSpeed, SB_CTL, MINVEL, MAXVEL, FALSE);
            SetScrollPos(hSpeed, SB_CTL, lSpeed, TRUE);

            // Retrieve a handle to the OK push button control.
            hOK = GetDlgItem(hDlg, ID_OK);
//            dxInit(hDlg);


            return TRUE;
        case WM_HSCROLL:
            // Process scroll bar input, adjusting the lSpeed
            // value as appropriate.
            switch (LOWORD(wParam)) {
                case SB_PAGEUP:
                case SB_LINEUP:
                    --lSpeed;
                    break;

                case SB_PAGEDOWN:
                case SB_LINEDOWN:
                    ++lSpeed;
                    break;

                case SB_THUMBPOSITION:
                    lSpeed = HIWORD(wParam);
                    break;

                case SB_BOTTOM:
                    lSpeed = MINVEL;
                    break;

                case SB_TOP:
                    lSpeed = MAXVEL;
                    break;

                case SB_THUMBTRACK:
                case SB_ENDSCROLL:
                    return TRUE;
            }

            if ((int) lSpeed <= MINVEL)
                lSpeed = MINVEL;
            if ((int) lSpeed >= MAXVEL)
                lSpeed = MAXVEL;

            SetScrollPos((HWND) lParam, SB_CTL, lSpeed, TRUE);
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_OK: {
                    HRESULT hr = StringCchPrintf(szTemp, 20, "%ld", lSpeed);
                    if (SUCCEEDED(hr))
                        WritePrivateProfileString(szAppName, szRedrawSpeed,
                                                  szTemp, szIniFile);
                }

                    // Write the current redraw speed variable to
                    // the .ini file.
                case ID_CANCEL:
                    EndDialog(hDlg, LOWORD(wParam) == ID_OK);
                    return TRUE;
            }
    }

    return FALSE;
}

LRESULT WINAPI ScreenSaverProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HDC hdc;      // device-context handle
    static RECT rc;       // RECT structure
    static UINT uTimer;   // timer identifier

    switch (message) {
        case WM_CREATE:
            // Retrieve the application name from the .rc file.
            LoadString(hMainInstance, idsAppName, szAppName, 80 * sizeof(TCHAR));

            // Retrieve the .ini (or registry) file name.
            LoadString(hMainInstance, idsIniFile, szIniFile, MAXFILELEN * sizeof(TCHAR));

            // TODO: Add error checking to verify LoadString success
            //       for both calls.

            // Retrieve any redraw speed data from the registry.
            lSpeed = GetPrivateProfileInt(szAppName, szRedrawSpeed,
                                          DEFVEL, szIniFile);

//            f3000 = new F3000();
            renderer = new DXRenderer(hwnd);
//            dxInit(hwnd);
            break;

        case WM_ERASEBKGND:
            renderer->clear();
            return 1;

        case WM_DESTROY:
            // When the WM_DESTROY message is issued, the screen saver
            // must destroy any of the timers that were set at WM_CREATE
            // time.
            if (uTimer)
                KillTimer(hwnd, uTimer);

//            delete f3000;
            delete renderer;
            break;

        case WM_PAINT:
            renderer->render(hwnd);
            return 0;

        default:
            break;
    }

// DefScreenSaverProc processes any messages ignored by ScreenSaverProc.
    return DefScreenSaverProc(hwnd, message, wParam, lParam);
}

BOOL WINAPI RegisterDialogClasses(HANDLE hInst) {
    return TRUE;
}
