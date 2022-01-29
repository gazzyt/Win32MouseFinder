#pragma once

#include <cstdlib>
#include <queue>

#include "MouseMoveRecord.h"

template<typename T>
class MouseMoveProcessor
{
public:
	MouseMoveProcessor()
	:	m_movements(), m_minMovement(20), m_windowSize(1000)
	{}

	MouseMoveProcessor(const MouseMoveProcessor<T>& rhs) = delete;
	MouseMoveProcessor(MouseMoveProcessor<T>&& rhs) = delete;
	MouseMoveProcessor<T>& operator=(const MouseMoveProcessor<T>& rhs) = delete;
	MouseMoveProcessor<T>& operator=(MouseMoveProcessor<T>&& rhs) = delete;

	// Returns true if the mouse has been waggled
	bool ProcessMovement(LONG xDistance)
	{
		if (abs(xDistance) < m_minMovement)
			return false;

		if (!m_movements.empty())
		{
			if (HaveSameSigns(xDistance, m_movements.back().XDistance))
			{
				// Mouse is still moving the same direction - not interesting
				return false;
			}
		}

		auto currentTickCount = T::GetTickCount();
		m_movements.push(MouseMoveRecord{currentTickCount, xDistance});

		auto maximumAge = currentTickCount - m_windowSize;

		// Remove any mouse movements that are older than maximumAge
		while (!m_movements.empty() && m_movements.front().TimeStamp < maximumAge)
		{
			m_movements.pop();
		}

		return m_movements.size() > 2;
	}

private:
	static bool HaveSameSigns(LONG a, LONG b)
	{
		return ((a ^ b) >= 0);
	}

private:
	std::queue<MouseMoveRecord> m_movements;
	LONG m_minMovement;
	ULONGLONG m_windowSize;
};
