#pragma once
#include "Physics.hpp"

#define UID unsigned int
using namespace Phys;

namespace Objects
{
	struct Entity //our entity is essentially a line which is transformed on the xyz axis
	{
		Point3 StandingPosition;
		Vector3 PointingAt;

		unsigned int Type; //for hitbox sizing purposes, consider it your 'character' type. Size parameter would be set based on Type
		Vector3 Size; //hitbox

		UID unique_id; //identifier
		unsigned int Hitpoints = 100;
	};


	//we assume shots go in a straight line forever until hitting some wall or target
	//in real implementations, a shot would likely fall into a derived class of an Entity (its still an Entity)
	struct Shot
	{
		Point3 StartPosition;
		Vector3 Direction;

		unsigned int TimeElapsed = 0; //our equations should be as a function of time, since targets can be moving while a shot travels through the air

		Point3 IntersectPoint;
		bool Collided = false;

		Line3 FinalTrajectory; //when a shot has collided with a target we can fill in the Line it creates from its starting position to collision. if no collision this tends towards +/- infinity

		Entity* Owner;
	};

	Shot* ShootAtDirection(Entity* Shooter, Vector3 aim); //regular shoot in direction 
}