#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

// Actor class, base class
class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, StudentWorld *newWorld) : GraphObject(imageID, startX, startY, startDirection, size, depth)
    {
        world = newWorld;
    }

    StudentWorld *getWorld()
    {
        return world;
    }

    virtual ~Actor() {}
    virtual bool aliveCheck();
    virtual bool deadCheck();
    virtual void setDead();
    virtual void setAlive();

    virtual void dosomething() = 0;

private:
    StudentWorld *world;
    bool alive;
    bool dead;
};

// Person class for anything that can move
class Agent : public Actor
{
public:
    Agent(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, StudentWorld *newWorld) : Actor(imageID, startX, startY, startDirection, size, depth, newWorld)
    {
        setVisible(true);
    }

    virtual void setHealth(int health);
    virtual int getHealth();
    virtual void damage(int val);

private:
    int num_health;
};

class ActivatingObject : public Actor
{
public:
    ActivatingObject(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, StudentWorld *World, int soundToPlay, bool activateOnPlayer, bool activateOnProtester, bool initallyActive) : Actor(imageID, startX, startY, startDirection, size, depth, World) {}

    bool isVisible();
    bool isPickable();
    void setPickup(bool pickup);

    void setTicksToLive();

private:
    bool visible;
    bool pickable;
};

// Iceman class, the player
class Iceman : public Agent
{
public:
    Iceman(int startX, int startY, StudentWorld *World) : Agent(IID_PLAYER, startX, startY, right, 1.0, 0, World)
    {
        setVisible(true);
        setHealth(10);
        setAlive();
    }
    virtual ~Iceman() {}
    virtual void dosomething();
};

// Ice class
class Ice : public Actor
{ // Ice does not do anything so just the constructor is fine
public:
    Ice(int startX, int startY, StudentWorld *World) : Actor(IID_ICE, startX, startY, right, 0.25, 3, World)
    {
        setVisible(true);
    }

    virtual ~Ice() {}

    virtual void dosomething() {}
};

class BarrelOfOil : public ActivatingObject
{
public:
    BarrelOfOil(int startX, int startY, StudentWorld *World) : ActivatingObject(IID_BARREL, startX, startY, right, 1.0, 2, World, SOUND_FOUND_OIL, true, false, false)
    {
        setVisible(true);
        setAlive();
    }
    virtual ~BarrelOfOil() {}
    virtual void dosomething();
    // virtual bool objectiveItem() const;
};

#endif // ACTOR_H_
