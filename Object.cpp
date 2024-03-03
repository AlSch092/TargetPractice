#include "Object.hpp"

using namespace Objects;

Shot* Entity::ShootInDirection(Vector3 CurrentAimDirection)
{
	Shot* shot = new Shot();

	shot->Direction = CurrentAimDirection;
	shot->StartPosition.X = StandingPosition.X;
	shot->StartPosition.Y = StandingPosition.Y;
	shot->StartPosition.Z = StandingPosition.Z;

	return shot;
}