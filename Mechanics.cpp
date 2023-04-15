#include "Mechanics.hpp"

bool Mechanics::IsColliding(Point3 StartPosition, Vector3 ShootDirection, Point3 TargetPosition, Vector3 TargetWidthHeightDepth) {

	// Check if the shot starts inside the target
	if (StartPosition.X >= TargetPosition.X - TargetWidthHeightDepth.X / 2.0f && StartPosition.X <= TargetPosition.X + TargetWidthHeightDepth.X / 2.0f
		&& StartPosition.Y >= TargetPosition.Y - TargetWidthHeightDepth.Y / 2.0f && StartPosition.Y <= TargetPosition.Y + TargetWidthHeightDepth.Y / 2.0f
		&& StartPosition.Z >= TargetPosition.Z - TargetWidthHeightDepth.Z / 2.0f && StartPosition.Z <= TargetPosition.Z + TargetWidthHeightDepth.Z / 2.0f)
		return true;

	// Check if the shot intersects any of the target's faces
	float tmin = -INFINITY, tmax = INFINITY;

	if (ShootDirection.X != 0.0f)
	{
		float tx1 = (TargetPosition.X - TargetWidthHeightDepth.X / 2.0f - StartPosition.X) / ShootDirection.X;
		float tx2 = (TargetPosition.X + TargetWidthHeightDepth.X / 2.0f - StartPosition.X) / ShootDirection.X;
		tmin = std::max(tmin, std::min(tx1, tx2));
		tmax = std::min(tmax, std::max(tx1, tx2));
	}
	if (ShootDirection.Y != 0.0f)
	{
		float ty1 = (TargetPosition.Y - TargetWidthHeightDepth.Y / 2.0f - StartPosition.Y) / ShootDirection.Y;
		float ty2 = (TargetPosition.Y + TargetWidthHeightDepth.Y / 2.0f - StartPosition.Y) / ShootDirection.Y;
		tmin = std::max(tmin, std::min(ty1, ty2));
		tmax = std::min(tmax, std::max(ty1, ty2));
	}
	if (ShootDirection.Z != 0.0f)
	{
		float tz1 = (TargetPosition.Z - TargetWidthHeightDepth.Z / 2.0f - StartPosition.Z) / ShootDirection.Z;
		float tz2 = (TargetPosition.Z + TargetWidthHeightDepth.Z / 2.0f - StartPosition.Z) / ShootDirection.Z;
		tmin = std::max(tmin, std::min(tz1, tz2));
		tmax = std::min(tmax, std::max(tz1, tz2));
	}

	if (tmax < std::max(0.0f, tmin))
		return false;

	return true;
}

UID Mechanics::GetEntityInShotLine(Entity* shooter, Shot* shot)
{
	for each (Entity* e in EntList)
	{
		if (Mechanics::IsColliding(shot->StartPosition, shot->Direction, e->StandingPosition, e->Size) && shooter->unique_id != e->unique_id)
		{
			printf("Player %d's shot hit Entity %d!\n", shooter->unique_id, e->unique_id);
			return e->unique_id;
		}
	}

	return 0;
}


Shot* Mechanics::EntityShootInDirection(Entity* Shooter, Vector3 aim)
{
	Shot* shot = new Shot();

	shot->Direction = aim;
	shot->StartPosition.X = Shooter->StandingPosition.X;
	shot->StartPosition.Y = Shooter->StandingPosition.Y;
	shot->StartPosition.Z = Shooter->StandingPosition.Z;

	if (Mechanics::GetEntityInShotLine(Shooter, shot))
	{
		printf("Player %d hit another player!\n", Shooter->unique_id);

		//decrease enemy hitpoints, or whatever logic
	}

	return shot; //remember to clean up memory after you're done using the structure
}

double Mechanics::Cheater::GetCorrectiveAngle(Shot* s, Entity* e)
{
	// Calculate the vector from start position to target position
	Vector3 toTarget = { e->StandingPosition.X - s->StartPosition.X, e->StandingPosition.Y - s->StartPosition.Y, e->StandingPosition.Z - s->StartPosition.Z };

	// Calculate the magnitude of the vector from start position to target position
	double toTargetMag = sqrt(pow(toTarget.X, 2) + pow(toTarget.Y, 2) + pow(toTarget.Z, 2));

	// Normalize the direction vector
	double dirMag = sqrt(pow(s->Direction.X, 2) + pow(s->Direction.Y, 2) + pow(s->Direction.Z, 2));

	Vector3 normDir = { s->Direction.X / dirMag, s->Direction.Y / dirMag, s->Direction.Z / dirMag };

	// Calculate the dot product of the direction vector and the vector from start position to target position
	double dotProd = normDir.X * toTarget.X + normDir.Y * toTarget.Y + normDir.Z * toTarget.Z;

	// Calculate the angle between the direction vector and the vector from start position to target position
	double angle = acos(dotProd / toTargetMag);

	// Convert the angle from radians to degrees
	angle = angle * 180 / M_PI;

	return angle;
}

Vector3 Mechanics::Cheater::GetCorrectiveVector(Shot* s, Entity* e, float speed)
{
	// Calculate the vector from start position to target position
	Vector3 toTarget = { e->StandingPosition.X - s->StartPosition.X, e->StandingPosition.Y - s->StartPosition.Y, e->StandingPosition.Z - s->StartPosition.Z };

	// Calculate the magnitude of the vector from start position to target position
	double toTargetMag = sqrt(pow(toTarget.X, 2) + pow(toTarget.Y, 2) + pow(toTarget.Z, 2));

	// Normalize the direction vector
	double dirMag = sqrt(pow(s->Direction.X, 2) + pow(s->Direction.Y, 2) + pow(s->Direction.Z, 2));

	Vector3 normDir = { s->Direction.X / dirMag, s->Direction.Y / dirMag, s->Direction.Z / dirMag };

	// Calculate the dot product of the direction vector and the vector from start position to target position
	double dotProd = normDir.X * toTarget.X + normDir.Y * toTarget.Y + normDir.Z * toTarget.Z;

	// Calculate the angle between the direction vector and the vector from start position to target position
	double angle = acos(dotProd / toTargetMag);

	// Calculate the time required for the projectile to reach the target
	double time = toTargetMag / (speed * sin(angle));

	// Calculate the required velocity vector
	Vector3 VelocityVec = { (toTarget.X / time - 0.5 * 0 * time) / cos(angle),
		(toTarget.Y / time - 0.5 * 9.8 * time) / cos(angle),
		(toTarget.Z / time - 0.5 * 0 * time) / cos(angle) };

	return VelocityVec;
}