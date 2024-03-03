#pragma once
#define NOMINMAX
#define M_PI   3.14159265358979323846264338327950288

#include "Object.hpp"
#include <list>
#include <algorithm>

using namespace Objects;
using namespace std;

namespace Mechanics //normally would be server-sided
{
	static std::list<Entity*> EntList = std::list<Entity*>(); //'global' entity list

	bool IsColliding(Point3 StartPosition, Vector3 ShootDirection, Point3 TargetPosition, Vector3 TargetWidthHeightDepth);

	list<UID> GetEntityInShotLine(std::list<Entity*> PlayerList, Entity* shooter, Shot* shot);

	double GetCorrectiveAngle(Shot* s, Entity* Target); //calculate the vector needed to hit a target
	Vector3 GetCorrectiveVector(Shot* s, Entity* e, float speed);
}