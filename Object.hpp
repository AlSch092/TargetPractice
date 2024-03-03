#pragma once
#include "Physics.hpp"

using namespace Phys;
using namespace std;

#define UID unsigned int

//In an actual production settings, Object class would be base/abstract class , then inherited by Entity class.
//In this example we are just looking at the raw data for shots fired from the server-side point of view
namespace Objects 
{
	//we assume shots go in a straight line forever until hitting some wall or target
	//in real implementations, a shot would likely fall into a derived class of Object
	struct Shot
	{
		unsigned int ProjectileId;

		Point3 StartPosition;
		Vector3 Direction;

		unsigned int TimeElapsed = 0; //our equations should be as a function of time, since targets can be moving while a shot travels through the air

		Point3 IntersectPoint;
		bool Collided = false;

		Line3 FinalTrajectory; //when a shot has collided with a target we can fill in the Line it creates from its starting position to collision. if no collision this tends towards +/- infinity
	};


	class Entity //our entity is essentially a line which is transformed on the xyz axis
	{
	public:
		Entity(int id) { unique_id = id; }

		Point3 StandingPosition;
		Vector3 PointingAt;

		unsigned int Type; //for hitbox sizing purposes, consider it your 'character' type. Size parameter would be set based on Type
		Vector3 Size; //hitbox

		UID unique_id; //identifier
		unsigned int Hitpoints = 100;

		Shot* ShootInDirection(Vector3 aim);
	};

}