//AlSch092 @ github
#include "Simulation.hpp"

//for such tests we are assuming this is data recieved the server-side
void Simulation::RunAllTestCases()
{
	printf("\n[INFO - Simulation::Run] Testing dataset-related techniques: \n");

	Entity* potential_cheater = new Entity(1);

	//set is linear until last point, where it suddenly skips past our threshold amount: our second method will detect it as cheating but first method will pass
	list<Point2> mouseAim_preShot = { { 1.0f, 2.0f }, { 2.0f, 4.0f }, { 3.0f, 6.0f }, { 4.0f, 8.0f }, { 5.0f, 10.0f }, { 15.0f, 15.0f } };

	if (WasPlayersAimLinearFunction(potential_cheater, mouseAim_preShot))
	{
		printf("Player %d's aim was linear: possibly cheating.\n", potential_cheater->UniqueId);
		potential_cheater->FlaggedAsCheater = true;
	}
	else
	{
		printf("The dataset of player %d's shot aim looks okay!\n", potential_cheater->UniqueId);
	}

	if (AreFramesSkipped(mouseAim_preShot, 10.0f))
	{
		printf("Player's aim skips faster than our threshold allows, possibly cheating.\n");
		potential_cheater->FlaggedAsCheater = true;
	}

	list<Point2> mouseAim_StraightLineNorth = { { 0.0f, 0.0f }, { 0.0f, 4.0f }, { 0.0f, 6.0f }, { 0.0f, 8.0f }, { 0.0f, 10.0f }, { 0.0f, 12.0f } };

	if (WasPlayersAimLinearFunction(potential_cheater, mouseAim_StraightLineNorth))
	{
		printf("Player %d's aim was linear: possibly cheating.\n", potential_cheater->UniqueId);
	}
	else
	{
		printf("The dataset of player %d's shot aim looks ok!\n", potential_cheater->UniqueId);
	}

	//last 5 points in dataset are colinear
	list<Point2> mouseAim_Sample = { { 1, 1 }, { 1, 3 }, { 2, 4 }, { 4, 4 }, { 5, 6 }, { 6, 4 }, { 7, 6 }, { 8, 5 }, { 9, 10 }, { 14, 14 }, { 16, 12 }, { 18, 12 }, { 16, 14 }, { 17, 15 }, { 18, 16 }, { 19, 17 }, { 20, 18 }, { 21, 19 } };

	int colinear_threshold = 5; //colinear points within the dataset could indicate a cheat tool is auto-correcting aim to hit targets last-second

	if (HasColinearPoints(mouseAim_Sample, colinear_threshold))
	{
		printf("Player %d's aim was likely auto-corrected by a cheat to hit the target: %d points were found to be colinear\n", potential_cheater->UniqueId);
		potential_cheater->FlaggedAsCheater = true;
	}

	delete potential_cheater;
}

//::HasColinearPoints helps us detect datasets where a player is aiming at another player and a cheat tool corrects their aim last second
// -> Checks if a linear function is within a subset of points
//threshold is the number of points in a row within the dataset to test. 
bool Simulation::HasColinearPoints(list<Point2> mouseDragOffsets, int threshold)
{
	bool result = false;
	Point2* Points = new Point2[mouseDragOffsets.size()];

	size_t i = 0;

	for (const auto& point : mouseDragOffsets)
		Points[i++] = point;

	for (int j = 0; j < i - threshold; j++)
	{
		bool isLinear = Phys::IsFunctionLinear(&Points[j], threshold);

		if (isLinear)
			result = true;
	}

	delete[] Points;
	return result;
}

//the previous X frames from the PoV of a player shooting: 
//if the function/line created by a player's 'drag' of the mouse is linear, it's probably a cheater
// -> if the tangents equal at every point in this set, the function is linear and thus "unnatural" 
bool Simulation::WasPlayersAimLinearFunction(Entity* actor, list<Point2> mouseDragOffsets)
{
	if (mouseDragOffsets.size() < 2 )
	{
		printf("WasPlayersAimLinearFunction: Data set anomaly\n");
		return false;
	}

	if (actor == NULL)
	{
		printf("Actor pointer was NULL\n");
		return false;
	}

	Point2* Points = new Point2[mouseDragOffsets.size()];

	size_t i = 0;

	for (const auto& point : mouseDragOffsets) 
		Points[i++] = point;
	
	bool isLinear = Phys::IsFunctionLinear(Points, mouseDragOffsets.size());

	delete Points;

	actor->FlaggedAsCheater = true;
	return isLinear;
}

bool Simulation::AreFramesSkipped(list<Point2> mouseDragOffsets, double threshold)
{
	if (mouseDragOffsets.size() < 2)
	{
		printf("AreFramesSkipped: Data set anomaly\n");
		return false;
	}

	Point2* Points = new Point2[mouseDragOffsets.size()];

	size_t i = 0;

	for (const auto& point : mouseDragOffsets)
		Points[i++] = point;

	//get distance between each consequtive point, since aiming creates points over time.
	//usually we expect to get N points over X milliseconds, with there being a max distance the mouse can move within that time frame. 
	for (int i = 0; i < mouseDragOffsets.size() - 1; i++)  
	{
		double distance = CalculateDistance(Points[i], Points[i + 1]);
		if (distance > threshold) 
		{
			delete[] Points;
			return true; // Found an invalid point
		}
	}

	delete[] Points;
	return false;
}

void Simulation::TestBasicPhysics() //simple 3d space tests, not related to data set tests
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
			printf("[GAME] Player %d's shot hit %d!\n", e->UniqueId, id);
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

