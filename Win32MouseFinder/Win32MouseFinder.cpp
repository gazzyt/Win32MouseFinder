// Win32MouseFinder.cpp : Defines the entry point for the application.
//

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


#include "framework.h"
#include "Win32MouseFinder.h"

#include "TickTimeProvider.h"
#include "MouseMoveProcessor.h"
#include "BigMouse.h"
#include "AutoRun.h"
#include "ErrorUtil.h"

#define MAX_LOADSTRING 100
#define TIMER_DURATION_MS 1000

UINT const WMAPP_NOTIFYCALLBACK = WM_APP + 1;
constexpr UINT_PTR TIMER_ID = 2;

// https://social.msdn.microsoft.com/Forums/vstudio/en-US/1c3be04f-32ed-4f63-9289-557d4426f65b/shellnorifyicon-question?forum=vcgeneral
#ifdef _DEBUG
// {954B7474-3CB8-4B73-9429-E3FA5C954DBF}
static const GUID NotificationIconGuid =
{ 0x954b7474, 0x3cb8, 0x4b73, { 0x94, 0x29, 0xe3, 0xfa, 0x5c, 0x95, 0x4d, 0xbf } };
#else
// {7CDAB978-8037-4CA4-B945-56A9692413FB}
static const GUID NotificationIconGuid =
{ 0x7cdab978, 0x8037, 0x4ca4, { 0xb9, 0x45, 0x56, 0xa9, 0x69, 0x24, 0x13, 0xfb } };
#endif // DEBUG


// Global Variables:
HINSTANCE hInst;                                // current instance
HWND hMainWindow;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
constexpr size_t ccStringBuffer = 255;
TCHAR szStringBuffer[ccStringBuffer];
MouseMoveProcessor<TickTimeProvider> mouseMoveProcessor;
BigMouse bigMouse;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL                CreateNotificationIcon(HWND hwnd);
BOOL                DestroyNotificationIcon();
void                ShowContextMenu(HWND hwnd, POINT pt);
BOOL                RegisterRawMouseInput(HWND hWnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32MOUSEFINDER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32MOUSEFINDER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32MOUSEFINDER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32MOUSEFINDER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hMainWindow = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hMainWindow)
   {
      return FALSE;
   }

   CreateNotificationIcon(hMainWindow);
   RegisterRawMouseInput(hMainWindow);
   ShowWindow(hMainWindow, SW_HIDE);
   UpdateWindow(hMainWindow);

   return TRUE;
}

BOOL CreateNotificationIcon(HWND hwnd)
{
    NOTIFYICONDATA nid = { sizeof(nid) };
    nid.hWnd = hwnd;
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_SHOWTIP | NIF_GUID;
    nid.guidItem = NotificationIconGuid;
    nid.uCallbackMessage = WMAPP_NOTIFYCALLBACK;
    nid.uVersion = NOTIFYICON_VERSION_4;
    LoadIconMetric(hInst, MAKEINTRESOURCE(IDI_WIN32MOUSEFINDER), LIM_SMALL, &nid.hIcon);
    LoadString(hInst, IDS_TOOLTIP, nid.szTip, ARRAYSIZE(nid.szTip));

    // Add the icon
    if (!Shell_NotifyIcon(NIM_ADD, &nid))
    {
        ErrorUtil:: ShowErrorDialog(hMainWindow, TEXT("Shell_NotifyIcon(NIM_ADD) failed"));
    }

    // Set the version
    return Shell_NotifyIcon(NIM_SETVERSION, &nid);
}

BOOL DestroyNotificationIcon()
{
    NOTIFYICONDATA nid = { sizeof(nid) };
    nid.uFlags = NIF_GUID;
    nid.guidItem = NotificationIconGuid;
    return Shell_NotifyIcon(NIM_DELETE, &nid);
}

void ShowContextMenu(HWND hwnd, POINT pt)
{
    HMENU hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDC_CONTEXTMENU));
    if (hMenu)
    {
        CheckMenuItem(hMenu, IDM_AUTORUN, MF_BYCOMMAND | (AutoRun::IsAutoRunEnabled() ? MF_CHECKED : MF_UNCHECKED));
        HMENU hSubMenu = GetSubMenu(hMenu, 0);
        if (hSubMenu)
        {
            UINT uFlags = TPM_RIGHTBUTTON;
            if (GetSystemMetrics(SM_MENUDROPALIGNMENT) != 0)
            {
                uFlags |= TPM_RIGHTALIGN;
            }
            else
            {
                uFlags |= TPM_LEFTALIGN;
            }

            SetForegroundWindow(hwnd);
            TrackPopupMenuEx(hSubMenu, uFlags, pt.x, pt.y, hwnd, NULL);
        }
        DestroyMenu(hMenu);
    }
}

BOOL RegisterRawMouseInput(HWND hWnd)
{
    RAWINPUTDEVICE mouse;

    mouse.usUsagePage = 0x01;   // HID_USAGE_PAGE_GENERIC
    mouse.usUsage = 0x02;       // HID_USAGE_GENERIC_MOUSE
    mouse.dwFlags = RIDEV_INPUTSINK;
    mouse.hwndTarget = hWnd;

    return RegisterRawInputDevices(&mouse, 1, sizeof(mouse));
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        DestroyNotificationIcon();
        PostQuitMessage(0);
        break;

    case WMAPP_NOTIFYCALLBACK:
        switch (LOWORD(lParam))
        {
        case WM_CONTEXTMENU:
            POINT cursorPos;
            auto ret = GetCursorPos(&cursorPos);
            ShowContextMenu(hWnd, cursorPos);
            break;
        }
        break;

    case WM_INPUT:
        {
            UINT dwSize;

            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
            LPBYTE lpb = new BYTE[dwSize];
            if (lpb == nullptr)
            {
                return 0;
            }

            if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
            {
                OutputDebugString(TEXT("GetRawInputData did not return expected size"));
            }

            RAWINPUT* raw = (RAWINPUT*)lpb;

            if (raw->header.dwType == RIM_TYPEMOUSE)
            {
                if (raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
                {
                    HRESULT hResult = StringCchPrintf(szStringBuffer, ccStringBuffer,
                        TEXT("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"),
                        raw->data.mouse.usFlags,
                        raw->data.mouse.ulButtons,
                        raw->data.mouse.usButtonFlags,
                        raw->data.mouse.usButtonData,
                        raw->data.mouse.ulRawButtons,
                        raw->data.mouse.lLastX,
                        raw->data.mouse.lLastY,
                        raw->data.mouse.ulExtraInformation);

                    //OutputDebugString(szStringBuffer);

                    if (mouseMoveProcessor.ProcessMovement(raw->data.mouse.lLastX))
                    {
                        OutputDebugString(TEXT("Triggered!\n"));
                        bigMouse.Show(hInst, hMainWindow, IDB_MOUSE200);
                        auto result = SetTimer(hWnd, TIMER_ID, TIMER_DURATION_MS, NULL);
                        if (result == 0)
                            OutputDebugString(TEXT("Timer creation failed\n"));
                    }

                    bigMouse.UpdatePosition(hMainWindow);
                }
                else
                {
                    OutputDebugString(TEXT("Mouse event was not MOUSE_MOVE_RELATIVE - ignoring"));
                }
            }
            else
            {
                OutputDebugString(TEXT("It's not a mouse"));
            }

            delete[] lpb;
        }
        break;

    case WM_TIMER:
        KillTimer(hWnd, TIMER_ID);
        bigMouse.Hide();
        OutputDebugString(TEXT("Timer expired\n"));
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


