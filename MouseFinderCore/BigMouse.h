#pragma once

#include "framework.h"
#include "Settings.h"

class BigMouse
{
public:
	BigMouse(const Settings& settings) : m_settings(settings), m_bigMouse(0)
	{}

	void Show(HINSTANCE hInstance, HWND parent, unsigned int bitmapResource);
	void Hide();
	void UpdatePosition(HWND parent);

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static ATOM RegisterClass(HINSTANCE hInstance, HWND parent);

private:
	static constexpr const TCHAR* m_wndClassName = TEXT("WIN32MOUSEFINDER_BIGMOUSE");
	const Settings& m_settings;
	HWND m_bigMouse;
	static ATOM m_wndClass;
	static unsigned int m_bitmapResource;
	static HANDLE m_mouseBitmap;
	static constexpr int m_width = 200;
	static constexpr int m_height = 200;
	static constexpr int m_widthOffset = 66;
	static constexpr int m_heightOffset = 22;

};
