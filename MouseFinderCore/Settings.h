#pragma once

#include "framework.h"

class Settings
{
public:
	void Load();
	void Save() const;

	bool IsHideMouseEnabled() const { return m_hideMouseEnabled; }
	void SetHideMouseEnabled(bool value) { m_hideMouseEnabled = value; }
	void ToggleHideMouseEnabled() { m_hideMouseEnabled = !m_hideMouseEnabled; }

	int GetSensitivity() const { return m_sensitivity; }
	void SetSensitivity(int sensitivity) { m_sensitivity = sensitivity; }

private:
	static DWORD ReadSetting(HKEY hSettingsKey, LPCTSTR keyName, DWORD defaultValue);
	static void WriteSetting(HKEY hSettingsKey, LPCTSTR keyName, DWORD value);


private:
	bool m_hideMouseEnabled = false;
	int m_sensitivity = 0;
};
