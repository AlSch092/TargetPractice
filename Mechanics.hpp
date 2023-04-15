#pragma once
#define NOMINMAX
#define M_PI   3.14159265358979323846264338327950288

#include "Object.hpp"
#include <list>
#include <algorithm>

using namespace Objects;

namespace Mechanics //normally would be server-sided
{
	static std::list<Entity*> EntList = std::list<Entity*>(); //'global' entity list

	UID GetEntityInShotLine(Entity* shooter, Shot* shot);

	bool IsColliding(Point3 StartPosition, Vector3 ShootDirection, Point3 TargetPosition, Vector3 TargetWidthHeightDepth);

	Shot* EntityShootInDirection(Entity* Shooter, Vector3 aim);

	namespace Cheater
	{
		double GetCorrectiveAngle(Shot* s, Entity* Target); //this is our 'aimbot' - should calculate the vector needed to hit a target when our shot is not on target.
		Vector3 GetCorrectiveVector(Shot* s, Entity* e, float speed);
	}
}