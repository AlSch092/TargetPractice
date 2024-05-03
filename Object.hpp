//AlSch092 @ github
#pragma once
#include "Physics.hpp"
#include "Dataset.hpp"

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

		Point3 IntersectPoint; //shots are infinite vectors if they hit no objects, 
		bool Collided = false;

		Line3 FinalTrajectory; //when a shot has collided with a target we can fill in the Line it creates from its starting position to collision. if no collision this tends towards +/- infinity
	};


	class Entity //our entity is essentially a line which is transformed on the xyz axis
	{
		public:
		Entity(int id) { UniqueId = id; }

		Point3 StandingPosition = { 0.0f, 0.0f , 0.0f };;
		Vector3 PointingAt = { 0.0f, 0.0f , 0.0f };;

		unsigned int Type = 0; //for hitbox sizing purposes, consider it your 'character' type. Size parameter would be set based on Type
		Vector3 Size = { 0.0f, 0.0f , 0.0f}; //hitbox

		UID UniqueId = 0; //identifier

		bool FlaggedAsCheater = false;

		Shot* ShootInDirection(Vector3 aim);

		Dataset<Point2>* AimData = nullptr;
	};

}