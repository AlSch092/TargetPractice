//AlSch092 @ github
#pragma once
#include "Mechanics.hpp"
#include <stdio.h>


template<typename T>
class Dataset
{
public:
	
	list<T> GetDataSet() { return this->Data; }

	void AddData(T data) { Data.push_back(data); }

private:
	list<T> Data;
};

namespace Simulation
{
	void RunAllTestCases();

	bool AreFramesSkipped(list<Point2> mouseDragOffsets, double threshold);
	bool WasPlayersAimLinearFunction(Entity* actor, list<Point2> mouseDragOffsets); //first test on data inputs to detect cheating: no mouse drag should be perfectly linear
	bool HasColinearPoints(list<Point2> mouseDragOffsets, int threshold);

	void TestBasicPhysics(); //unrelated to dataset tests
}

