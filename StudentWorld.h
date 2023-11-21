#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include "Actor.h"
#include <vector>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	// Generate the level
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{
	}

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	void setPositions(int x, int y, char actorName);
	int getPositions(int x, int y);

private:
	int num_ice = 3616;
	Ice *iceObject[3616];
	Iceman *iceManObject;
	vector<Actor *> Actors;
	char Positions[64][64];
};

#endif // STUDENTWORLD_H_
