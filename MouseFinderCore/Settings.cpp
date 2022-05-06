#include "Settings.h"

#include "ErrorUtil.h"

static LPCTSTR SettingsKey = _TEXT("Software\\Win32MouseFinder");
static LPCTSTR HideMouseValue = _TEXT("HideMouse");
static LPCTSTR SensitivityValue = _TEXT("Sensitivity");

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
		m_hideMouseEnabled = ReadSetting(hSettingsKey, HideMouseValue, 0);
		m_sensitivity = ReadSetting(hSettingsKey, SensitivityValue, 0);

		RegCloseKey(hSettingsKey);
	}
}

void Settings::Save() const
{
	auto hSettingsKey = OpenSettingsKey();

	if (hSettingsKey)
	{
		WriteSetting(hSettingsKey, HideMouseValue, m_hideMouseEnabled);
		WriteSetting(hSettingsKey, SensitivityValue, m_sensitivity);

		RegCloseKey(hSettingsKey);
	}
}

DWORD Settings::ReadSetting(HKEY hSettingsKey, LPCTSTR keyName, DWORD defaultValue)
{
	DWORD valueRead;
	DWORD valueReadSize = sizeof(valueRead);
	auto getValueResult = RegGetValue(hSettingsKey, NULL, keyName,
		RRF_RT_REG_DWORD | RRF_ZEROONFAILURE,
		NULL,
		&valueRead,
		&valueReadSize);

	return (getValueResult == ERROR_SUCCESS) ? valueRead : defaultValue;
}

void Settings::WriteSetting(HKEY hSettingsKey, LPCTSTR keyName, DWORD value)
{
	RegSetValueEx(hSettingsKey, keyName, 0, REG_DWORD,
		reinterpret_cast<BYTE*>(&value),
		sizeof(value));
}
