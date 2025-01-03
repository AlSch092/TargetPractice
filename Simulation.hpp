//AlSch092 @ github
#pragma once
#include "Mechanics.hpp"
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

namespace Simulation
{
	void RunAllTestCases();

	bool AreFramesSkipped(vector<Point2> mouseDragOffsets, double threshold);
	bool WasPlayersAimLinearFunction(Entity* actor, vector<Point2> mouseDragOffsets); //first test on data inputs to detect cheating: no mouse drag should be perfectly linear
	bool HasColinearPoints(vector<Point2> mouseDragOffsets, int threshold);
	bool AllPointsPerfectlyRounded(vector<Point2> points);

    static Dataset<Point2>* ReadDataPointsFromFile(std::string filename)
    {
        Dataset<Point2>* ds = new Dataset<Point2>();

        stringstream ss;

        ifstream file(filename);
        double x, y;

        while (file >> x >> y)
        {
            ds->AddData({ x, y });
        }

        return ds;
    }
	
	void TestBasicPhysics(); //unrelated to dataset tests
}

