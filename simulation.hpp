#pragma once
#define NOMINMAX
#define UID unsigned int
#define M_PI   3.14159265358979323846264338327950288

#include <math.h>
#include <time.h>
#include <Windows.h> 
#include <list>
#include <algorithm>

using namespace std;

//if this gets much larger we'll split up each struct into its own file

namespace Phys
{
	struct Vector3 //vector represents line going in some direction forever
	{
		FLOAT X, Y, Z;
	};

	struct Point3
	{
		FLOAT X, Y, Z;
	};

	struct Line3
	{
		Point3 Position;
		Vector3 Direction;
	};
}

using namespace Phys;

namespace Objects
{
	struct Entity //our entity is essentially a line which is transformed on the xyz axis
	{
		Vector3 StandingPosition;
		Vector3 PointingAt;

		int Type; //for hitbox sizing purposes, consider it your 'character' type. Size parameter would be set based on Type
		Vector3 Size; //hitbox

		UID unique_id; //identifier
		unsigned int Hitpoints = 100;
	};

	//we assume shots go in a straight line forever until hitting some wall or target
	//in real implementations, a shot would likely fall into a derived class of an Entity (its still an Entity)
	struct Shot
	{
		Vector3 StartPosition;
		Vector3 Direction;

		unsigned int TimeElapsed = 0; //our equations should be as a function of time, since targets can be moving while a shot travels through the air

		Point3 IntersectPoint;
		bool Collided = false;

		Line3 FinalTrajectory; //when a shot has collided with a target we can fill in the Line it creates from its starting position to collision. if no collision this tends towards +/- infinity

		Entity* Owner;
	};

	Shot* ShootAtDirection(Entity* Shooter, Vector3 aim); //regular shoot in direction 
}

using namespace Objects;

namespace Mechanics //normally would be server-sided
{
	static std::list<Entity*> EntList = std::list<Entity*>(); //'global' entity list

	UID GetEntityInShotLine(Entity* shooter, Shot* shot);

	bool IsColliding(Vector3 StartPosition, Vector3 ShootDirection, Vector3 TargetPosition, Vector3 TargetWidthHeightDepth);

	namespace Cheater
	{
		double GetCorrectiveAngle(Shot* s, Entity* Target); //this is our 'aimbot' - should calculate the vector needed to hit a target when our shot is not on target.
		Vector3 GetCorrectiveVector(Shot* s, Entity* e, float speed);
	}
}