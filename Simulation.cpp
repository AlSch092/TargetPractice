#include "Simulation.hpp"

void Simulation::Run()
{
	TestBasicPhysics(); //see if our simple 3D space works

	Entity* potential_cheater = new Entity(1);

	std::list<Point2> dragOffsets_preShot = { { 0.0f, 2.0f }, { 2.0f, 4.0f }, { 3.0f, 6.0f }, { 4.0f, 8.0f } };

	if (WasPlayersAimLinearFunction(potential_cheater, dragOffsets_preShot))
	{
		printf("Player %d's aim was perfectly linear on the last shot", potential_cheater->unique_id);
	}
	else
	{
		printf("The dataset of player %d's shot aim looks ok!\n");
	}
}


//the previous 1 second of data from the PoV of a player shooting: 
//if the function/line created by a player's 'drag' of the mouse is linear, it's probably a cheater
// -> if the tangents equal at every point in this set, the function is linear and thus "unnatural" 
bool Simulation::WasPlayersAimLinearFunction(Entity* actor, std::list<Point2> mouseDragOffsets)
{
	if (mouseDragOffsets.size() == 0)
	{
		printf("Data set anomaly\n");
		return false;
	}

	Point2* Points = new Point2[mouseDragOffsets.size()];

	size_t i = 0;

	for (const auto& point : mouseDragOffsets) 
		Points[i++] = point;
	
	bool isLinear = Phys::IsFunctionLinear(Points, mouseDragOffsets.size());

	delete Points;

	return isLinear;
}


void Simulation::TestBasicPhysics()
{
	Entity* e = new Entity(1);
	e->StandingPosition = { -2.0, 2.0, 2.0 };
	e->PointingAt = { 2.0, -2.0, -2.0 };

	Entity* t = new Entity(2);
	t->StandingPosition = { 0.0, 0.0, 0.0 };
	t->Size = { 1.0, 1.0, 1.0 };

	Mechanics::EntList.push_back(e);
	Mechanics::EntList.push_back(t);

	if (Mechanics::IsColliding(e->StandingPosition, e->PointingAt, t->StandingPosition, t->Size)) //specific position check including 'size' of the entity as a hitbox
	{
		printf("[GAME] Shot hit target\n");
	}
	else
	{
		printf("[GAME] Shot missed target, try again.\n");
	}

	Shot* s = new Shot();
	s->StartPosition = e->StandingPosition; //for simplicity's sake
	s->Direction = { 2.0, -2.0, -2.0 };

	std::list<UID> EntityIdCollision = Mechanics::GetEntityInShotLine(Mechanics::EntList, e, s); //we can then subtract health from the entity hit

	delete s; s = NULL;

	if (EntityIdCollision.size() > 0)
	{
		for each (UID id in EntityIdCollision)
			printf("[GAME] Player %d's shot hit %d!\n", e->unique_id, id);
	}
	
	Shot* s_1 = new Shot();
	s_1->StartPosition = t->StandingPosition;
	s_1->Direction = { 2.0, 4.0, 2.0 };

	double AutoAimAngle = Mechanics::GetCorrectiveAngle(s_1, e); //shoot at E from T
	Vector3 AutoAimVector = Mechanics::GetCorrectiveVector(s_1, e, 100.0);

	printf("[INFO] Required angle to hit target: {%f}\n", AutoAimAngle);
	printf("[INFO] Required vector to hit target: {%f,%f,%f}\n", AutoAimVector.X, AutoAimVector.Y, AutoAimVector.Z); //confirmed working -> gets us the vector needed to hit the target

	delete s_1; s_1 = nullptr;
	delete e; e = nullptr;
	delete t; t = nullptr;
}

