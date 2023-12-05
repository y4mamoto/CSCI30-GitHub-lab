#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GameController.h"
#include <string>
#include "Actor.h"
#include <vector>
#include <cmath>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	// Generate the level
	StudentWorld(std::string assetDir) : GameWorld(assetDir) {}

	virtual ~StudentWorld() {}

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	virtual bool removeIce(int x, int y);

	virtual bool nearIcemanCheck(int x, int y, int radius);

	virtual void deleteDeadActor();

	// virtual void addActor(Actor* a);

private:
	Ice *iceObject[64][60];
	Iceman *iceManObject;
	vector<Actor *> Actors;
	BarrelOfOil *barrelObject;
};

#endif // STUDENTWORLD_H_
