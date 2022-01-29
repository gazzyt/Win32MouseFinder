#include "ErrorUtil.h"

TCHAR ErrorUtil::szStringBuffer[ccStringBuffer];


void ErrorUtil::ShowErrorDialog(HWND hParentWindow, LPCTSTR message)
{
    HRESULT hResult = StringCchPrintf(szStringBuffer, ccStringBuffer,
        TEXT("Error: %04x"),
        GetLastError());

    MessageBox(hParentWindow, message, szStringBuffer, MB_OK | MB_ICONERROR | MB_APPLMODAL);
}
