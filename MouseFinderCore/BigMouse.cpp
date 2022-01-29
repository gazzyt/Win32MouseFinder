#include "BigMouse.h"
#include "ErrorUtil.h"

ATOM BigMouse::m_wndClass(0);

void BigMouse::Show(HINSTANCE hInstance, HWND parent)
{
	if (m_wndClass == 0)
	{
        m_wndClass = RegisterClass(hInstance, parent);
	}

    if (m_bigMouse == 0)
    {
        m_bigMouse = CreateWindowEx(WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW, m_wndClassName, TEXT("LORDY"), WS_POPUPWINDOW | WS_VISIBLE | WS_BORDER | WS_CHILD, 400, 400, 400, 400, NULL, NULL, hInstance, NULL);
        SetLayeredWindowAttributes(m_bigMouse, 0, (255 * 70) / 100, LWA_ALPHA); // 70% alpha
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

        if (!MoveWindow(m_bigMouse, currentMousePos.x, currentMousePos.y, 400, 400, FALSE))
        {
            ErrorUtil::ShowErrorDialog(parent, TEXT("MoveWindow for BigMouse failed"));
        }
    }
}

LRESULT CALLBACK BigMouse::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HBRUSH hBrush;

    switch (message)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            hBrush = CreateSolidBrush(RGB(200, 0, 0));
            FillRect(hdc, &ps.rcPaint, hBrush);
            DeleteObject(hBrush);
            // TODO: Add any drawing code that uses hdc here...
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
