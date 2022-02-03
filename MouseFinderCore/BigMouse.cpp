#include "BigMouse.h"
#include "ErrorUtil.h"

ATOM BigMouse::m_wndClass(0);
HANDLE BigMouse::m_mouseBitmap(0);
unsigned int BigMouse::m_bitmapResource(0);

void BigMouse::Show(HINSTANCE hInstance, HWND parent, unsigned int bitmapResource)
{
	if (m_wndClass == 0)
	{
        m_wndClass = RegisterClass(hInstance, parent);
	}

    if (m_bigMouse == 0)
    {
        m_bitmapResource = bitmapResource;
        m_bigMouse = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW, m_wndClassName, nullptr, WS_POPUP | WS_VISIBLE | WS_CHILD, 0, 0, m_width, m_height, NULL, NULL, hInstance, NULL);
        SetLayeredWindowAttributes(m_bigMouse, RGB(255, 0, 0), 0, LWA_COLORKEY); // set red as transparent colour
        ShowWindow(m_bigMouse, SW_SHOW);
    }
}

void BigMouse::Hide()
{
    if (m_bigMouse != 0)
    {
        DestroyWindow(m_bigMouse);
        m_bigMouse = 0;
    }
}

void BigMouse::UpdatePosition(HWND parent)
{
    if (m_bigMouse != 0)
    {
        POINT currentMousePos;
        if (!GetCursorPos(&currentMousePos))
        {
            ErrorUtil::ShowErrorDialog(parent, TEXT("GetCursorPos for BigMouse failed"));
            return;
        }

        if (!MoveWindow(m_bigMouse, currentMousePos.x, currentMousePos.y, m_width, m_height, FALSE))
        {
            ErrorUtil::ShowErrorDialog(parent, TEXT("MoveWindow for BigMouse failed"));
        }
    }
}

LRESULT CALLBACK BigMouse::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
            m_mouseBitmap = LoadImage(GetModuleHandle(nullptr),
                MAKEINTRESOURCE(m_bitmapResource),
                IMAGE_BITMAP,
                0,
                0,
                LR_DEFAULTCOLOR);
            break;

        case WM_DESTROY:
            DeleteObject(m_mouseBitmap);
            m_mouseBitmap = 0;
            break;

        case WM_PAINT:
        {
            BITMAP bm;
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            HDC hdcMem = CreateCompatibleDC(hdc);
            HGDIOBJ hbmOld = SelectObject(hdcMem, m_mouseBitmap);
            BitBlt(hdc, 0, 0, m_width, m_height, hdcMem, 0, 0, SRCCOPY);
            SelectObject(hdcMem, hbmOld);

            EndPaint(hWnd, &ps);
        }
        break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

ATOM BigMouse::RegisterClass(HINSTANCE hInstance, HWND parent)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = BigMouse::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = m_wndClassName;
    wcex.hIconSm = NULL;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    ATOM retval = RegisterClassEx(&wcex);

    if (retval == 0)
    {
        ErrorUtil::ShowErrorDialog(parent, TEXT("RegisterClassEx for BigMouse failed"));
    }

    return retval;
}
