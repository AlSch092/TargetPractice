//AlSch092 @ github
#include "Simulation.hpp"


//for such tests we are assuming this is data recieved the server-side
void Simulation::RunAllTestCases()
{
	cout << endl << "[INFO - Simulation::Run] Testing dataset - related techniques : " << endl;

	//this dataset is linear until last point, where it suddenly skips past our threshold amount: our second method will detect it as cheating but first method will pass
	Dataset<Point2>* Dataset_MouseAim = Simulation::ReadDataPointsFromFile("./first_test.txt");

	int currentId = 1;
	Entity* potential_cheater = new Entity(currentId++);
	potential_cheater->AimData = Dataset_MouseAim;

	if (WasPlayersAimLinearFunction(potential_cheater, Dataset_MouseAim->GetDataset())) //should pass okay
	{
		cout << "Player's aim was linear: possibly cheating: " << potential_cheater->UniqueId << endl;
		potential_cheater->FlaggedAsCheater = true;
	}
	else
	{
		cout << "The dataset of player " << potential_cheater->UniqueId << "'s shot aim looks okay!" << endl;
	}

	if (AreFramesSkipped(Dataset_MouseAim->GetDataset(), 10.0f)) //should flag
	{
		cout << "Player's aim skips faster than our threshold allows, possibly cheating." << endl;
		potential_cheater->FlaggedAsCheater = true;
	}

	Dataset<Point2>* Dataset_MouseAimLinear = Simulation::ReadDataPointsFromFile("./second_test.txt");

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
	Dataset<Point2>* Dataset_MouseAimColinear = Simulation::ReadDataPointsFromFile("./third_test.txt");

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
bool Simulation::HasColinearPoints(vector<Point2> mouseDragOffsets, int threshold)
{
	bool foundLinearSubset = false;

	for (int i = 0; i < mouseDragOffsets.size(); i++)
	{
		if (i + threshold > mouseDragOffsets.size())
			break;

		vector<Point2> subset(mouseDragOffsets.begin() + i, mouseDragOffsets.begin() + i + threshold); //proabably quite slow computationally since we need to copy N subsets

		if (Phys::IsFunctionLinear(subset, threshold))
		{
			foundLinearSubset = true;
			break;
		}
	}

	return foundLinearSubset; //perfectly linear means someone is probably cheating
}

//the previous X frames from the PoV of a player shooting: 
//if the function/line created by a player's 'drag' of the mouse is fully linear, it's probably a cheater
// -> if the tangents equal at every point in this set, the function is linear and thus "unnatural" 
bool Simulation::WasPlayersAimLinearFunction(Entity* actor, vector<Point2> mouseDragOffsets)
{
	if (mouseDragOffsets.size() < 2 )
	{
		cout << "WasPlayersAimLinearFunction: Data set anomaly, size of data set is too small!" << endl;
		return false;
	}

	if (actor == NULL)
	{
		printf("Actor pointer was NULL\n");
		return false;
	}

	bool isLinear = Phys::IsFunctionLinear(mouseDragOffsets, mouseDragOffsets.size());

	actor->FlaggedAsCheater = true;
	return isLinear;
}

bool Simulation::AreFramesSkipped(vector<Point2> mouseDragOffsets, double threshold)
{
	if (mouseDragOffsets.size() < 2)
	{
		printf("AreFramesSkipped: Data set anomaly, size of data set is too small!\n");
		return false;
	}

	//get distance between each consequtive point, since aiming creates points over time.
	//usually we expect to get N points over X milliseconds, with there being a max distance the mouse can move within that time frame. 
	for (int i = 0; i < mouseDragOffsets.size() - 1; i++)  
	{
		double distance = CalculateDistance(mouseDragOffsets[i], mouseDragOffsets[i + 1]);

		if (distance > threshold) 
		{
			return true; // Found an invalid point
		}
	}

	return false;
}

bool Simulation::AllPointsPerfectlyRounded(vector<Point2> points)
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


