//AlSch092 @ github
#pragma once
#include "Mechanics.hpp"
#include <stdio.h>

namespace Simulation
{
	void Run();
	void TestBasicPhysics();

	bool AreFramesSkipped(list<Point2> mouseDragOffsets, double threshold);
	bool WasPlayersAimLinearFunction(Entity* actor, list<Point2> mouseDragOffsets); //first test on data inputs to detect cheating: no mouse drag should be perfectly linear
}