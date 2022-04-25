#pragma once

#include "framework.h"

class ErrorUtil
{
public:
	// Show error from GetLastError()
	static void ShowErrorDialog(HWND hParentWindow, LPCTSTR message);

	// Show error from LSTATUS
	static void ShowErrorDialog(HWND hParentWindow, LPCTSTR message, const LSTATUS status);

private:
	static constexpr size_t ccStringBuffer = 255;
	static TCHAR szStringBuffer[];

};

