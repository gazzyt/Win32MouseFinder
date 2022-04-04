#include "AutoRun.h"

#include "framework.h"
#include "ErrorUtil.h"

namespace AutoRun
{
	static LPCTSTR RunKey = _TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	static LPCTSTR RunValue = _TEXT("Win32MouseFinder");

	bool IsAutoRunEnabled()
	{
		bool retval = false;
		HKEY hkRun;
		auto result = RegOpenKeyEx(HKEY_CURRENT_USER, RunKey, 0, KEY_READ, &hkRun);
		if (result == ERROR_SUCCESS)
		{
			auto valueResult = RegQueryValueEx(hkRun, RunValue, NULL, NULL, NULL, NULL);

			retval = valueResult == ERROR_SUCCESS;

			RegCloseKey(hkRun);
		}
		return retval;
	}

	void ToggleAutoRun()
	{
		if (IsAutoRunEnabled())
		{
			DisableAutoRun();
		}
		else
		{
			EnableAutoRun();
		}
	}

	void EnableAutoRun()
	{
		TCHAR buffer[MAX_PATH];
		auto getExeFileNameResult = GetModuleFileName(NULL, buffer, sizeof(buffer) / sizeof(buffer[0]));
		if (getExeFileNameResult >= sizeof(buffer) / sizeof(buffer[0]))
		{
			ErrorUtil::ShowErrorDialog(NULL, _TEXT("Path to exe is longer than MAX_PATH"));
			return;
		}

		// We need the path surrounded with double quotes for the registry value
		TCHAR exePath[MAX_PATH + 2];
		if (FAILED(StringCchPrintf(exePath, sizeof(exePath) / sizeof(exePath[0]), _TEXT("\"%s\""), buffer)))
		{
			ErrorUtil::ShowErrorDialog(NULL, _TEXT("StringCchPrintf failed"));
			return;
		}

		size_t exePathBytes = 0; // Excludes terminating null
		if (FAILED(StringCbLength(exePath, sizeof(exePath), &exePathBytes)))
		{
			ErrorUtil::ShowErrorDialog(NULL, _TEXT("StringCbLength failed"));
			return;
		}
		
		// We got the path to the executable ok
		HKEY hkRun;
		auto registryResult = RegOpenKeyEx(HKEY_CURRENT_USER, RunKey, 0, KEY_READ | KEY_SET_VALUE, &hkRun);
		if (registryResult != ERROR_SUCCESS)
		{
			ErrorUtil::ShowErrorDialog(NULL, _TEXT("Failed to open registry key"));
			return;
		}

		registryResult = RegSetValueEx(hkRun, RunValue, 0, REG_SZ, 
			reinterpret_cast<BYTE*>(exePath),
			exePathBytes + sizeof(TCHAR) // Includes terminating null
		);

		if (registryResult != ERROR_SUCCESS)
		{
			ErrorUtil::ShowErrorDialog(NULL, _TEXT("Failed to set registry value"));
		}

		RegCloseKey(hkRun);
	}

	void DisableAutoRun()
	{
		HKEY hkRun;
		auto registryResult = RegOpenKeyEx(HKEY_CURRENT_USER, RunKey, 0, KEY_READ | KEY_SET_VALUE, &hkRun);
		if (registryResult != ERROR_SUCCESS)
		{
			ErrorUtil::ShowErrorDialog(NULL, _TEXT("Failed to open registry key"));
			return;
		}

		registryResult = RegDeleteValue(hkRun, RunValue);
		if (registryResult != ERROR_SUCCESS)
		{
			ErrorUtil::ShowErrorDialog(NULL, _TEXT("Failed to delete registry value"));
		}
	}
};
