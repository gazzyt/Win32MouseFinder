#pragma once

#include "framework.h"

struct MouseMoveParameters
{
	constexpr MouseMoveParameters(size_t minWags, LONG minMovement, ULONGLONG windowSize)
		: MinWags(minWags), MinMovement(minMovement), WindowSize(windowSize)
	{}

	size_t MinWags;			// Minimum number of back and forths required to trigger big mouse
	LONG MinMovement;		// Minimum mouse movement that counts as a wag - basically a proxy for mouse speed
	ULONGLONG WindowSize;	// Number of milliseconds during which MinWags must occur to trigger big mouse
};
