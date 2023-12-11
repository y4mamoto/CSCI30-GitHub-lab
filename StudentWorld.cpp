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

	int G = getLevel() * 25 + 300;

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
			addActor(new WaterPool(x, y, this, ticks));
		}
	}

	vector<Actor *>::iterator it;
	for (it = Actors.begin(); it != Actors.end(); it++)
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
		return GWSTATUS_FINISHED_LEVEL;

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

	vector<Actor *>::iterator it;
	for (it = Actors.begin(); it != Actors.end();)
	{
		delete (*it);
		it = Actors.erase(it);
	}
}

// Function that remove the ice when the player comes into contact of ice
void StudentWorld::removeIce(int x, int y)
{
	for (int i = 0; i < 64; i++)
		for (int j = 0; j < 60; j++)
		{
			if (iceObject[i][j] != nullptr)
				if (iceObject[i][j]->getX() >= x && iceObject[i][j]->getX() <= x + 3 && iceObject[i][j]->getY() >= y && iceObject[i][j]->getY() <= y + 3)
				{
					GameController::getInstance().playSound(SOUND_DIG);
					delete iceObject[i][j];
					iceObject[i][j] = nullptr;
				}
		}
    for (int i = 30; i < 34; i++)                   //implementation for chasm
        for (int j = 10; j < 60; j++)
        {
            delete iceObject[i][j];                 //sets ice in line of chasm to null
            iceObject[i][j] = nullptr;
        }
}		
}

bool StudentWorld::nearIcemanCheck(int x, int y, int radius)
{
	int priorRadiusCalc;
	priorRadiusCalc = pow(iceManObject->getX() - x, 2) + pow(iceManObject->getY() - y, 2);
	if (sqrt(priorRadiusCalc) <= radius)
		return true;

	return false;
}

void StudentWorld::deleteDeadActor()
{
	vector<Actor *>::iterator it;
	for (it = Actors.begin(); it != Actors.end();)
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
	vector<Actor *>::iterator it;
	for (it = Actors.begin(); it != Actors.end(); it++)
	{
		double preDistance = pow((*it)->getX() - x, 2) + pow((*it)->getY() - y, 2);
		if (sqrt(preDistance) <= radius)
		{
			(*it)->GraphObject::setVisible(true);
		}
	}
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
	vector<Actor *>::iterator it;
	for (it = Actors.begin(); it != Actors.end(); it++)
	{
		int preDistance = pow((*it)->getX() - x, 2) + pow((*it)->getY() - y, 2);
		if (sqrt(preDistance) <= radius)
			return false;
	}

	return true;
}

void StudentWorld::setDisplayText()
{
	int level = getLevel();
	int lives = getLives();
	int health = iceManObject->getHealth() * 10;
	int water = IcemanObject()->getWater();
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
	stats.append(spaces);
	return stats;
}
