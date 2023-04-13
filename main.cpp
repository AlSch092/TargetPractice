/*
TargetPractice - a simple simulation of shooting game physics and how we can detect abnormal play
By AlSch092, for educational/research purposes
*/
#include "simulation.hpp"

using namespace Objects;

int main(int argc, char** argv)
{
	Entity* e = new Entity();
	e->unique_id = 1; //if we want to get fancy we can make the Entity constructor iterate the entity list and auto-assign the unique_id as +1 of the last id, working like a counter.
	e->StandingPosition = { 10.0, 11.0, 2.0 };
	e->PointingAt = { 5.0, 0.0, 4.0 };

	Entity* t = new Entity();
	t->unique_id = 2;
	t->StandingPosition = { 15.0, 10.0, 5.0 };
	t->Size = { 5.0, 2.5, 1.0 };

	Mechanics::EntList.push_back(e);
	Mechanics::EntList.push_back(t);

	if (Mechanics::IsColliding(e->StandingPosition, e->PointingAt, t->StandingPosition, t->Size)) //specific position check including 'size' of the entity as a hitbox
	{
		printf("[GAME] Shot hit target!\n");
	}
	else
	{
		printf("[GAME] Shot missed target, try again...\n");
	}

	Shot* s = new Shot();
	s->StartPosition = e->StandingPosition; //for simplicity's sake
	s->Direction = { 10.0, 5.0, -5.0 };

	UID EntityIdCollision = Mechanics::GetEntityInShotLine(e, s); //we can then subtract health from the entity hit

	delete s; s = NULL;

	if (EntityIdCollision)
	{
		printf("[GAME] Player %d's shot hit %d!", e->unique_id, EntityIdCollision);
	}

	Shot* s_1 = new Shot();
	s_1->StartPosition = t->StandingPosition;
	s_1->Direction = { 1.0, 2.0, 3.0 };

	double AutoAimAngle = Mechanics::Cheater::GetCorrectiveAngle(s_1, e);
	Vector3 AutoAimVector = Mechanics::Cheater::GetCorrectiveVector(s_1, e, 100.0);

	printf("[INFO] Required vector to hit target: {%f,%f,%f}\n", AutoAimVector.X, AutoAimVector.Y, AutoAimVector.Z);

	//now that our initial physics/tests work well, we can start trying to detect 'abnormal' shooting. what if the target was hit while aiming at a wrong angle?
	//...a work in progress

	system("pause");
	return 0;
}