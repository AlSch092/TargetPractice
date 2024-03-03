#pragma once
#include "Mechanics.hpp"
#include <stdio.h>

namespace Simulation
{
	void Run();
	void TestBasicPhysics();

	bool WasPlayersAimLinearFunction(Entity* actor, std::list<Point2> mouseDragOffsets);
}