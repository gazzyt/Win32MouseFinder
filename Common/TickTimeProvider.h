#pragma once

class TickTimeProvider
{
public:
	TickTimeProvider() = delete;

	static ULONGLONG GetTickCount()
	{
		return GetTickCount64();
	}

};
