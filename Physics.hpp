//AlSch092 @ github
#pragma once
#include <math.h>

namespace Phys
{
	struct Vector3 //a vector in 3d space (infinite mag)
	{
		double X, Y, Z;
	};

	struct Point3 //a point in 3d space
	{
		double X, Y, Z;
	};

	struct Line3 //finite line
	{
		Point3 Position;
		Vector3 Direction;
		double Length;
	};

	struct Point2
	{
		double X;
		double Y;
	};

	double CalculateSlope(Point2 p1, Point2 p2); //slope between two 2D points -> can be used for detecting first-person aiming anomalies
	double CalculateDistance(Point2 p1, Point2 p2);
	
	bool IsFunctionLinear(Point2 aimingPoints[], int num_points);
}