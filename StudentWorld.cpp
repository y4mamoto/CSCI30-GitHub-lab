#include "StudentWorld.h"
#include "GameConstants.h"
#include "GameController.h"
#include "GraphObject.h"
#include <string>
#include <cmath>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

GameWorld *createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init()
{ // Setup the level
	barrelPickedUp = 0;

	for (int x = 0; x < 64; x++)
		for (int y = 0; y < 60; y++)
		{
			iceObject[x][y] = new Ice(x, y, this);
		}

	num_barrel = min<int>(2 + getLevel(), 21);
	for (int i = 0; i < num_barrel;)
	{
		int x = random() % 61;
		while (x >= 27 && x <= 37)
			x = random() % 61;
		int y = random() % 57;
		if (spawnRangeCheck(x, y, 6))
		{
			barrelObject[i] = new BarrelOfOil(x, y, this);
			addActor(barrelObject[i]);
			i++;
		}
	}

	num_gold = max<int>(5 - getLevel() / 2, 2);
	for (int i = 0; i < num_gold;)
	{
		int x = random() % 61;
		while (x >= 27 && x <= 37)
			x = random() % 61;
		int y = random() % 57;
		if (spawnRangeCheck(x, y, 6))
		{
			goldObject[i] = (new Gold(x, y, this, false));
			addActor(goldObject[i]);
			i++;
		}
	}

	num_boulders = min<int>(getLevel() / 2 + 2, 9);
	for (int i = 0; i < num_boulders;)
	{
		int x = random() % 61;
		while (x >= 27 && x <= 37)
			x = random() % 61;
		int y = random() % 37 + 20;
		if (spawnRangeCheck(x, y, 6))
		{
			removeIce(x, y);
			boulderObject[i] = (new Boulder(x, y, this));
			addActor(boulderObject[i]);
			i++;
		}
	}

	iceManObject = new Iceman(30, 60, this); // Create Iceman
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	setDisplayText();

	int probabilityOfHardcore = min<int>(90, getLevel() * 10 + 30);
	int G = getLevel() * 25 + 300;

	num_protester = 0;
	max_protester = min<int>(15, 2 + getLevel() * 1.5);
	protester_ticks = max<int>(25, 200 - getLevel());

	if (num_protester == 0)
	{
		regular_protesterObject[0] = (new Regular_Protester(this));
		num_protester++;
	}

	if (random() % G + 1 <= 1)
	{
		int ticks = max<int>(100, 300 - 10 * getLevel());
		int chance = random() % 5 + 1;
		if (chance <= 1)
			addActor(new SonarKit(0, 60, this, ticks));
		else
		{
			int x = random() % 61;
			int y = random() % 57;
			if (noIceCheck(x, y))
			{
				addActor(new WaterPool(x, y, this, ticks));
			}
		}
	}

	for (auto it = Actors.begin(); it != Actors.end(); it++)
	{
		if ((*it)->aliveCheck())
		{
			(*it)->dosomething();
		}
	}
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

	deleteDeadActor();

	if (barrelPickedUp == num_barrel)
	{
		GameController::getInstance().playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() // Delete the entire level after it is finish
{
	// Delete Remaining Ice of the level
	for (int x = 0; x < 64; x++)
	{
		for (int y = 0; y < 60; y++)
		{
			if (iceObject[x][y] != nullptr)
			{
				delete iceObject[x][y];
				iceObject[x][y] = nullptr;
			}
		}
	}

	delete iceManObject; // Delete Iceman

	// Delete all remaining actors
	for (auto it = Actors.begin(); it != Actors.end();)
	{
		delete (*it);
		it = Actors.erase(it);
	}
}

// Function that remove the ice when the player comes into contact of ice
void StudentWorld::removeIce(int x, int y)
{
	for (int i = 60; i < 64; i++)
		for (int j = 60; j < 64; j++)
		{
			delete iceObject[i][j];
			iceObject[i][j] = nullptr;
		}

	for (int i = x; i < x + 4; ++i)
	{
		for (int j = y; j < y + 4; ++j)
		{
			if (x >= 0 && x < 61 && y >= 0 && y < 60)
			{
				delete iceObject[i][j];
				iceObject[i][j] = nullptr;
			}
		}
	}

	for (int i = 30; i < 34; i++) // implementation for chasm
		for (int j = 10; j < 60; j++)
		{
			delete iceObject[i][j]; // sets ice in line of chasm to null
			iceObject[i][j] = nullptr;
		}
}
bool StudentWorld::nearIcemanCheck(int x, int y, int radius)
{
	int iceManX = iceManObject->getX();
	int iceManY = iceManObject->getY();
	int priorRadiusCalc;
	priorRadiusCalc = pow(iceManX - x, 2) + pow(iceManY - y, 2);
	if (sqrt(priorRadiusCalc) <= radius)
		return true;

	return false;
}

void StudentWorld::deleteDeadActor()
{
	for (auto it = Actors.begin(); it != Actors.end();)
	{
		if ((*it)->deadCheck())
		{
			delete (*it);
			it = Actors.erase(it);
		}
		else
			it++;
	}
}

void StudentWorld::addActor(Actor *a)
{
	Actors.push_back(a);
}

void StudentWorld::pickupBarrel()
{
	barrelPickedUp++;
}

int StudentWorld::barrelLeft()
{
	return num_barrel - barrelPickedUp;
}

void StudentWorld::goldDrop(int x, int y)
{
	addActor(new Gold(x, y, this, true));
}

void StudentWorld::revealAllNearbyObjects(int x, int y, int radius)
{
	GameController::getInstance().playSound(SOUND_SONAR);

	for (auto &actor : Actors)
	{
		double preDistance = pow(actor->getX() - x, 2) + pow(actor->getY() - y, 2);
		if (sqrt(preDistance) <= radius)
		{
			actor->setVisible(true);
		}
	}
}

void StudentWorld::giveIcemanGold()
{
	iceManObject->addGold();
}

void StudentWorld::giveIceManSonar()
{
	iceManObject->addSonar();
}

void StudentWorld::giveIceManWater()
{
	iceManObject->addWater();
}

bool StudentWorld::spawnRangeCheck(int x, int y, int radius)
{
	for (auto it = Actors.begin(); it != Actors.end(); it++)
	{
		int preDistance = pow((*it)->getX() - x, 2) + pow((*it)->getY() - y, 2);
		if (sqrt(preDistance) <= radius)
			return false;
	}

	return true;
}

bool StudentWorld::noIceCheck(int x, int y)
{

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Ice *icePoint = iceObject[x + i][y + j];
			if (icePoint != nullptr)
				return false;
		}
	}
	return true;
}
bool StudentWorld::boulderCheck(int x, int y, GraphObject::Direction dir)
{
	for (int i = 0; i < num_boulders; ++i)
	{
		int boulderX = boulderObject[i]->getX();
		int boulderY = boulderObject[i]->getY();

		switch (dir)
		{

		case GraphObject::right:

			if (boulderX == x + 3 && inRangeCheck(boulderY, y, 3))
			{
				return true;
			}
			break;

		case GraphObject::left:
			if (boulderX == x && inRangeCheck(boulderY, y, 3))
			{
				return true;
			}
			break;

		case GraphObject::up:
			if (boulderY == y + 3 && inRangeCheck(boulderX, x, 3))
			{
				return true;
			}
			break;
		case GraphObject::down:
			if (boulderY == y && inRangeCheck(boulderX, x, 3))
			{
				return true;
			}
			break;
		default:
			break;
		}
	}
	return false;
}

bool StudentWorld::IceCheck(int x, int y, GraphObject::Direction dir)
{
	switch (dir)
	{
	case GraphObject::right:
		x += 1;
		break;
	case GraphObject::left:
		x -= 1;
		break;
	case GraphObject::up:
		y += 1;
		break;
	case GraphObject::down:
		y -= 1;
		break;
	default:
		break;
	}
	if (iceObject[x][y] != nullptr)
		return true;
	return false;
}

bool StudentWorld::inRangeCheck(int value, int target, int range)
{
	if (value >= target - range && value <= target + range)
	{
		return true;
	}
	else
		return false;
}

void StudentWorld::generateSquirt(int x, int y, GraphObject::Direction dir)
{
	addActor(new Squirt(x, y, dir, this));
}

void StudentWorld::decrementSpawnTicks()
{
	protester_ticks--;
}

void StudentWorld::setDisplayText()
{
	int level = getLevel();
	int lives = getLives();
	int health = iceManObject->getHealth() * 10;
	int water = iceManObject->getWater();
	int gold = iceManObject->getGold();
	int barrelIsLeft = barrelLeft();
	int sonarAMT = iceManObject->getSonar();
	int score = getScore();

	string s = statsFormat(level, lives, health, water, gold, barrelIsLeft, sonarAMT, score);
	setGameStatText(s);
}

string StudentWorld::statsFormat(int level, int lives, int health, int water, int gold, int barrelIsLeft, int sonarAMT, int Score)
{
	string stats;
	string spaces = "  ";
	string levelStr = to_string(level);
	stats.append("Lvl: ");
	if (levelStr.size() == 1)
		stats.append(" ");
	stats.append(levelStr);
	stats.append(" ");
	string livesStr = to_string(lives);
	stats.append("Lives: ");
	stats.append(livesStr);
	stats.append(spaces);
	string healthStr = to_string(health);
	stats.append("Hlth: ");
	if (healthStr.size() == 1)
		stats.append(spaces);
	if (healthStr.size() == 2)
		stats.append(" ");
	stats.append(healthStr);
	stats.append("%");
	stats.append(spaces);
	string waterStr = to_string(water);
	stats.append("Wtr: ");
	if (waterStr.size() == 1)
		stats.append(" ");
	stats.append(waterStr);
	stats.append(spaces);
	string goldStr = to_string(gold);
	stats.append("Gld: ");
	if (goldStr.size() == 1)
		stats.append(" ");
	stats.append(goldStr);
	stats.append(spaces);
	string barrelStr = to_string(barrelIsLeft);
	stats.append("Oil Left: ");
	if (barrelStr.size() == 1)
		stats.append(" ");
	stats.append(barrelStr);
	stats.append(spaces);
	string sonarStr = to_string(sonarAMT);
	stats.append("Sonar: ");
	if (sonarStr.size() == 1)
		stats.append(" ");
	stats.append(sonarStr);
	stats.append(spaces);
	string scoreStr = to_string(Score);
	stats.append("Scr: ");
	switch (scoreStr.size())
	{
	case 6:
		break;
	case 5:
		stats.append("0");
		break;
	case 4:
		stats.append("00");
		break;
	case 3:
		stats.append("000");
		break;
	case 2:
		stats.append("0000");
		break;
	case 1:
		stats.append("00000");
		break;
	default:
		stats.append("000000");
		break;
	}
	stats.append(scoreStr);
	return stats;
}
