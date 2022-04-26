#include "Settings.h"

#include "ErrorUtil.h"

static LPCTSTR SettingsKey = _TEXT("Software\\Win32MouseFinder");
static LPCTSTR HideMouseValue = _TEXT("HideMouse");

HKEY OpenSettingsKey()
{
	HKEY hSettingsKey = NULL;
	const auto result = RegCreateKeyEx(HKEY_CURRENT_USER, SettingsKey,
		0, // Reserved
		NULL, // lpClass
		REG_OPTION_NON_VOLATILE, // dwOptions
		KEY_READ | KEY_SET_VALUE, // samDesired
		NULL, // lpSecurityAttributes
		&hSettingsKey, // phkResult
		NULL);

	if (result == ERROR_SUCCESS)
	{
		return hSettingsKey;
	}
	else 
	{
		ErrorUtil::ShowErrorDialog(NULL, _TEXT("Failed to open or create settings key"), result);
		return NULL;
	}
}

void Settings::Load()
{
	auto hSettingsKey = OpenSettingsKey();

	if (hSettingsKey)
	{
		DWORD hideMouseValueRead;
		DWORD hideMouseValueReadSize = sizeof(hideMouseValueRead);
		auto getValueResult = RegGetValue(hSettingsKey, NULL, HideMouseValue,
			RRF_RT_REG_DWORD | RRF_ZEROONFAILURE,
			NULL,
			&hideMouseValueRead,
			&hideMouseValueReadSize);

		if (getValueResult == ERROR_SUCCESS)
		{
			m_hideMouseEnabled = hideMouseValueRead;
		}

		RegCloseKey(hSettingsKey);
	}
}

void Settings::Save() const
{
	auto hSettingsKey = OpenSettingsKey();

	if (hSettingsKey)
	{
		DWORD hideMouseValueToWrite = m_hideMouseEnabled;
		RegSetValueEx(hSettingsKey, HideMouseValue, 0, REG_DWORD,
			reinterpret_cast<BYTE *>(&hideMouseValueToWrite),
			sizeof(hideMouseValueToWrite));

		RegCloseKey(hSettingsKey);
	}
}
