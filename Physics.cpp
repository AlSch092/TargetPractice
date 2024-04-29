//AlSch092 @ github
#include "Physics.hpp"

using namespace Phys;

double Phys::CalculateSlope(Point2 p1, Point2 p2)  //calculate the slope between two points
{
	if (p1.X == p2.Y) //zero division exception			
		return 0;

	return (p2.Y - p1.Y) / (p2.X - p1.X);
}

bool Phys::IsFunctionLinear(Point2 points[], int num_points) {
	
	if (num_points < 2) //data set anomaly
		return false;
	
	double slope = Phys::CalculateSlope(points[0], points[1]); //first tangent slope in set, now check if the rest are equal

	for (int i = 1; i < num_points - 2; i++) 
	{
		double current_slope = Phys::CalculateSlope(points[i], points[i + 1]);
		if (current_slope != slope) 		
			return false; 		
	}

	return true; 
}

double Phys::CalculateDistance(Point2 p1, Point2 p2) 
{
	return sqrt(pow(p2.X - p1.X, 2) + pow(p2.Y - p1.Y, 2));
}