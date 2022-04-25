#include "ErrorUtil.h"

TCHAR ErrorUtil::szStringBuffer[ccStringBuffer];


void ErrorUtil::ShowErrorDialog(HWND hParentWindow, LPCTSTR message)
{
    HRESULT hResult = StringCchPrintf(szStringBuffer, ccStringBuffer,
        TEXT("Error: %04x"),
        GetLastError());

    MessageBox(hParentWindow, message, szStringBuffer, MB_OK | MB_ICONERROR | MB_APPLMODAL);
}

void ErrorUtil::ShowErrorDialog(HWND hParentWindow, LPCTSTR message, const LSTATUS status)
{
    const auto messageChars = FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        status,
        0,
        szStringBuffer,
        ccStringBuffer,
        NULL
    );

    if (messageChars == 0)
    {
        // FormatMessage failed
        ErrorUtil::ShowErrorDialog(NULL, _TEXT("FormatMessage failed"));
        szStringBuffer[0] = NULL; // Just in case FormatMessage didn't set a null terminator
    }

    MessageBox(hParentWindow, szStringBuffer, message, MB_OK | MB_ICONERROR | MB_APPLMODAL);
}
