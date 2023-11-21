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
	/*for(int i = 0; i < num_ice;i++){
		 for (int x = 0 ; x < 64; x++){
			 for(int y = 0; x < 60 ; y++){
				 iceObject[i] = new Ice(x,y,this);
			 }
		 }
	 }

	 for (int x = 30 ; x != 34; x++){
		 for (int y = 4; y !=60 ;y++)
			 Positions[x][y] = 't';
	 }*/
	iceManObject = new Iceman(30, 60, this);
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{ // Middle of the level
	if (iceManObject->aliveCheck())
	{
		iceManObject->dosomething();
	}
	if (!iceManObject->aliveCheck())
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() // Delete the entire level after it is finish
{
	for (int i = 0; i < num_ice; i++)
	{
		if (iceObject[i] != nullptr)
		{
			delete iceObject[i];
			iceObject[i] = nullptr;
		}
	}
}

/*void StudentWorld::setPositions(int x, int y, char actorName){
	for (int i = x ; i!= x + 4; x++)
		for (int j = y; j!=y+4; j++)
			Positions[i][j] = actorName;
}

int StudentWorld::getPositions(int x, int y){
	return Positions[x][y];
} */
