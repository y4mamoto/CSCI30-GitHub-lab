#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld *createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init()
{ // Setup the level
	for (int x = 0; x < 64; x++)
		for (int y = 0; y < 60; y++)
		{
			iceObject[x][y] = new Ice(x, y, this);
		}

	iceManObject = new Iceman(30, 60, this);
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{ // Middle of the level
	if (iceManObject->aliveCheck())
	{
		iceManObject->dosomething();
	}
	if (iceManObject->deadCheck())
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() // Delete the entire level after it is finish
{
	for (int x = 0; x < 64; x++)
		for (int y = 0; y < 60; y++)
		{
			if (iceObject[x][y] != nullptr)
			{
				delete iceObject[x][y];
				iceObject[x][y] = nullptr;
			}
		}

	delete iceManObject;
}

bool StudentWorld::iceCheck(int x, int y, GraphObject::Direction dir)
{
	switch (dir)
	{
	case GraphObject::up:
		for (int i = 0; i < 64; i++)
			for (int j = 0; j < 60; j++)
			{
				if (iceObject[i][j] != nullptr)
					if (iceObject[i][j]->getX() >= x && iceObject[i][j]->getX() <= x + 3 && iceObject[i][j]->getY() == y + 3)
						return true;
			}
		break;

	case GraphObject::down:
		for (int i = 0; i < 64; i++)
			for (int j = 0; j < 60; j++)
			{
				if (iceObject[i][j] != nullptr)
					if (iceObject[i][j]->getX() >= x && iceObject[i][j]->getX() <= x + 3 && iceObject[i][j]->getY() == y)
						return true;
			}
		break;

	case GraphObject::left:
		for (int i = 0; i < 64; i++)
			for (int j = 0; j < 60; j++)
			{
				if (iceObject[i][j]->getX() == x && iceObject[i][j]->getY() <= y && iceObject[i][j]->getY() <= y + 3)
					return true;
			}
		break;
	case GraphObject::right:
		for (int i = 0; i < 64; i++)
			for (int j = 0; j < 60; j++)
			{
				if (iceObject[i][j]->getX() == x + 3 && iceObject[i][j]->getY() <= y && iceObject[i][j]->getY() <= y + 3)
					return true;
			}
		break;
	}

	return false;
}

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
