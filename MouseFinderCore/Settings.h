#pragma once

#include "framework.h"

class Settings
{
public:
	void Load();
	void Save() const;

	bool IsHideMouseEnabled() const { return m_hideMouseEnabled; }
	void SetHideMouseEnabled(bool value) { m_hideMouseEnabled = value; }

private:
	bool m_hideMouseEnabled = false;
};
