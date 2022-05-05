#pragma once

#include <cstdlib>
#include <queue>

#include "MouseMoveRecord.h"
#include "MouseMoveParameters.h"

template<typename T>
class MouseMoveProcessor
{
public:
	MouseMoveProcessor()
	:	m_movements()
	{}

	MouseMoveProcessor(const MouseMoveProcessor<T>& rhs) = delete;
	MouseMoveProcessor(MouseMoveProcessor<T>&& rhs) = delete;
	MouseMoveProcessor<T>& operator=(const MouseMoveProcessor<T>& rhs) = delete;
	MouseMoveProcessor<T>& operator=(MouseMoveProcessor<T>&& rhs) = delete;

	// Returns true if the mouse has been waggled
	bool ProcessMovement(LONG xDistance)
	{
		int mouseSensitivity = 0;
		const MouseMoveParameters& params = mouseSensitivity < s_mouseMoveParametersCount ? s_mouseMoveParameters[mouseSensitivity] : s_mouseMoveParameters[0];

		if (abs(xDistance) < params.MinMovement)
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

		auto maximumAge = currentTickCount - params.WindowSize;

		// Remove any mouse movements that are older than maximumAge
		while (!m_movements.empty() && m_movements.front().TimeStamp < maximumAge)
		{
			m_movements.pop();
		}

		return m_movements.size() > params.MinWags;
	}

private:
	static bool HaveSameSigns(LONG a, LONG b)
	{
		return ((a ^ b) >= 0);
	}

private:
	// Sensitivity settings
	static constexpr MouseMoveParameters s_mouseMoveParameters[] =
	{
		MouseMoveParameters(2, 20, 1000), // high
		MouseMoveParameters(3, 30, 1000), // medium
		MouseMoveParameters(4, 40, 1000)  // low
	};
	static constexpr size_t s_mouseMoveParametersCount = sizeof(s_mouseMoveParameters) / sizeof(MouseMoveParameters);

private:
	std::queue<MouseMoveRecord> m_movements;
};
