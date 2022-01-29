#pragma once

#include "framework.h"

class ErrorUtil
{
public:
	static void ShowErrorDialog(HWND hParentWindow, LPCTSTR message);

private:
	static constexpr size_t ccStringBuffer = 255;
	static TCHAR szStringBuffer[];

};

