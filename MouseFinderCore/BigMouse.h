#pragma once

#include "framework.h"

class BigMouse
{
public:
	BigMouse() : m_bigMouse(0)
	{}

	void Show(HINSTANCE hInstance, HWND parent);
	void Hide();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static ATOM RegisterClass(HINSTANCE hInstance, HWND parent);

private:
	static constexpr const TCHAR* m_wndClassName = TEXT("WIN32MOUSEFINDER_BIGMOUSE");
	HWND m_bigMouse;
	static ATOM m_wndClass;

};
