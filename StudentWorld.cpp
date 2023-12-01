#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <cmath>
using namespace std;

GameWorld *createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init()
{ // Setup the level

	for (int x = 0; x < 64; x++)
	{
		if (x % 4 == 0)
			barrelObject = new BarrelOfOil(x, 30, this);
	}
	// Create the map with ice
	for (int x = 0; x < 64; x++)
		for (int y = 0; y < 60; y++)
		{
			iceObject[x][y] = new Ice(x, y, this);
		}

	iceManObject = new Iceman(30, 60, this); // Create Iceman
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{ // Middle of the level
	// Check if Iceman is alive, then dosomething will be called
	if (iceManObject->aliveCheck())
	{
		iceManObject->dosomething();
	}
	// Check if Iceman is dead, if true end the level and decrement a life
	if (iceManObject->deadCheck())
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() // Delete the entire level after it is finish
{
	// Delete Remaining Ice of the level
	for (int x = 0; x < 64; x++)
		for (int y = 0; y < 60; y++)
		{
			if (iceObject[x][y] != nullptr)
			{
				delete iceObject[x][y];
				iceObject[x][y] = nullptr;
			}
		}

	delete iceManObject; // Delete Iceman
}

// Function that remove the ice when the player comes into contact of ice
bool StudentWorld::removeIce(int x, int y)
{
	for (int i = 0; i < 64; i++)
		for (int j = 0; j < 60; j++)
		{
			if (iceObject[i][j] != nullptr)
				if (iceObject[i][j]->getX() >= x && iceObject[i][j]->getX() <= x + 3 && iceObject[i][j]->getY() >= y && iceObject[i][j]->getY() <= y + 3)
				{
					delete iceObject[i][j];
					iceObject[i][j] = nullptr;
					return true;
				}
		}
	return false;
}

bool StudentWorld::nearIcemanCheck(int x, int y, int radius)
{
	int priorRadiusCalc;
	priorRadiusCalc = pow(iceManObject->getX() - x, 2) + pow(iceManObject->getY() - y, 2);
	if (sqrt(priorRadiusCalc) >= radius)
		return true;

	return false;
}
