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

private:
	bool m_hideMouseEnabled = false;
};
