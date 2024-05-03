//AlSch092 @ github
#include "Simulation.hpp"

//for such tests we are assuming this is data recieved the server-side
void Simulation::RunAllTestCases()
{
	printf("\n[INFO - Simulation::Run] Testing dataset-related techniques: \n");

	//this dataset is linear until last point, where it suddenly skips past our threshold amount: our second method will detect it as cheating but first method will pass
	Dataset<Point2>* Dataset_MouseAim = new Dataset<Point2>(); 
	Dataset_MouseAim->AddData({ 1.0f, 2.0f });
	Dataset_MouseAim->AddData({ 2.0f, 4.0f });
	Dataset_MouseAim->AddData({ 3.0f, 6.0f });
	Dataset_MouseAim->AddData({ 4.0f, 8.0f });
	Dataset_MouseAim->AddData({ 5.0f, 10.0f });
	Dataset_MouseAim->AddData({ 15.0f, 15.0f });

	int currentId = 1;
	Entity* potential_cheater = new Entity(currentId++);
	potential_cheater->AimData = Dataset_MouseAim;

	if (WasPlayersAimLinearFunction(potential_cheater, Dataset_MouseAim->GetDataset()))
	{
		printf("Player %d's aim was linear: possibly cheating.\n", potential_cheater->UniqueId);
		potential_cheater->FlaggedAsCheater = true;
	}
	else
	{
		printf("The dataset of player %d's shot aim looks okay!\n", potential_cheater->UniqueId);
	}

	if (AreFramesSkipped(Dataset_MouseAim->GetDataset(), 10.0f))
	{
		printf("Player's aim skips faster than our threshold allows, possibly cheating.\n");
		potential_cheater->FlaggedAsCheater = true;
	}

	Dataset<Point2>* Dataset_MouseAimLinear = new Dataset<Point2>();
	Dataset_MouseAimLinear->AddData({ 0.0f, 0.0f });
	Dataset_MouseAimLinear->AddData({ 0.0f, 4.0f });
	Dataset_MouseAimLinear->AddData({ 0.0f, 6.0f });
	Dataset_MouseAimLinear->AddData({ 0.0f, 8.0f });
	Dataset_MouseAimLinear->AddData({ 0.0f, 10.0f });
	Dataset_MouseAimLinear->AddData({ 0.0f, 12.0f });

	Entity* potential_cheater_2 = new Entity(currentId++);
	potential_cheater_2->AimData = Dataset_MouseAimLinear;

	if (WasPlayersAimLinearFunction(potential_cheater, Dataset_MouseAimLinear->GetDataset()))
	{
		printf("Player %d's aim was linear: possibly cheating.\n", potential_cheater_2->UniqueId);
	}
	else
	{
		printf("The dataset of player %d's shot aim looks ok!\n", potential_cheater_2->UniqueId);
	}

	//last 5 points in dataset are colinear
	Dataset<Point2>* Dataset_MouseAimColinear = new Dataset<Point2>();
	Dataset_MouseAimColinear->AddData({ 0.0f, 0.0f });
	Dataset_MouseAimColinear->AddData({ 1.0f, 1.0f }); //not colinear
	Dataset_MouseAimColinear->AddData({ 1.0f, 3.0f });
	Dataset_MouseAimColinear->AddData({ 2.0f, 4.0f });
	Dataset_MouseAimColinear->AddData({ 4.0f, 4.0f });
	Dataset_MouseAimColinear->AddData({ 5.0f, 6.0f });
	Dataset_MouseAimColinear->AddData({ 6.0f, 4.0f });
	Dataset_MouseAimColinear->AddData({ 7.0f, 6.0f });
	Dataset_MouseAimColinear->AddData({ 8.0f, 5.0f });
	Dataset_MouseAimColinear->AddData({ 9.0f, 10.0f });
	Dataset_MouseAimColinear->AddData({ 14.0f, 14.0f });
	Dataset_MouseAimColinear->AddData({ 16.0f, 12.0f });
	Dataset_MouseAimColinear->AddData({ 18.0f, 12.0f });
	Dataset_MouseAimColinear->AddData({ 16.0f, 14.0f });

	Dataset_MouseAimColinear->AddData({ 17.0f, 15.0f }); //colinear
	Dataset_MouseAimColinear->AddData({ 18.0f, 16.0f });
	Dataset_MouseAimColinear->AddData({ 19.0f, 17.0f });
	Dataset_MouseAimColinear->AddData({ 20.0f, 18.0f });
	Dataset_MouseAimColinear->AddData({ 21.0f, 19.0f });

	Entity* potential_cheater_3 = new Entity(currentId++);
	potential_cheater_3->AimData = Dataset_MouseAimColinear;

	int colinear_threshold = 5; //colinear points within the dataset could indicate a cheat tool is auto-correcting aim to hit targets last-second -> a vector is formed by the cheat software from the current aim position to the target

	if (HasColinearPoints(Dataset_MouseAimColinear->GetDataset(), colinear_threshold))
	{
		printf("Player %d's aim was likely auto-corrected by a cheat to hit the target .. points were found to be colinear\n", potential_cheater_3->UniqueId);
		potential_cheater->FlaggedAsCheater = true;
	}

	if (AllPointsPerfectlyRounded(Dataset_MouseAimColinear->GetDataset()))
	{
		printf("All points in the dataset were perfectly rounded, implying some lazy cheat maker forgot to make their auto-aim human-like  \n");
	}

	delete Dataset_MouseAim;
	delete Dataset_MouseAimLinear;
	delete potential_cheater;
	delete potential_cheater_2;
	delete potential_cheater_3;
}

//::HasColinearPoints helps us detect datasets where a player is aiming at another player and a cheat tool corrects their aim last second during user click/weapon fired
// -> Checks if a linear function is within a subset of points
//threshold is the number of points in a row within the dataset to test. 
bool Simulation::HasColinearPoints(list<Point2> mouseDragOffsets, int threshold)
{
	bool result = false;
	int count = 0;

	Point2* pt_list = new Point2[mouseDragOffsets.size()]; //copy managed list to contiguous array such that we can compare subsets with changing index counts
	
	std::list<Point2>::iterator it;
	for (it = mouseDragOffsets.begin(); it != mouseDragOffsets.end(); ++it) 
	{
		pt_list[count].X = it._Ptr->_Myval.X;
		pt_list[count].Y = it._Ptr->_Myval.Y;
		count++;
	}

	for (int j = 0; j < count - threshold; j++)
	{
		bool isLinear = Phys::IsFunctionLinear(&pt_list[j], threshold); //perfectly linear means someone is probably cheating

		if (isLinear)
			result = true;
	}

	delete[] pt_list;
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

	delete[] Points;

	actor->FlaggedAsCheater = true;
	return isLinear;
}

bool Simulation::AreFramesSkipped(list<Point2> mouseDragOffsets, double threshold)
{
	if (mouseDragOffsets.size() < 2)
	{
		printf("AreFramesSkipped: Data set anomaly, size of data set is too small!\n");
		return false;
	}

	Point2* Points = new Point2[mouseDragOffsets.size()];

	size_t i = 0;

	for (const auto& point : mouseDragOffsets)
		Points[i] = point;

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

bool Simulation::AllPointsPerfectlyRounded(list<Point2> points)
{
	for (const auto& point : points)
	{
		if (!(point.X == (int)point.X && point.Y == (int)point.Y))
		{
			return false;
		}
	}

	return true; // All points are perfectly rounded
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


