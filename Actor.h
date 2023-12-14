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

    // virtual void dosomething();
    virtual void addGold() = 0;
    void setHealth(int health);
    int getHealth();
    void damage(int val);

private:
    int num_health;
};

class ActivatingObject : public Actor
{
public:
    ActivatingObject(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, StudentWorld *World, int soundToPlay, bool activateOnPlayer, bool activateOnProtester, bool initallyActive) : Actor(imageID, startX, startY, startDirection, size, depth, World) {}

    bool isPickable();
    bool isTemporary();
    void setTemporary(bool temporary);
    void setPickup(bool pickup);

    void setTicksToLive(int value);
    int getTicks() const;
    void removeTicks();

private:
    bool temp;
    bool pickable;
    int ticks;
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
        water = 999;
    }
    virtual void addGold();
    virtual void addSonar();
    virtual void addWater();
    unsigned int getGold() const;
    unsigned int getSonar() const;
    unsigned int getWater() const;

    virtual ~Iceman() {}
    virtual void dosomething();

private:
    int gold;
    int sonar;
    int water;
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
};

class Boulder : public Actor
{
public:
    Boulder(int startX, int startY, StudentWorld *world) : Actor(IID_BOULDER, startX, startY, down, 1.0, 1, world)
    {
        setVisible(true);
    }

    virtual ~Boulder() {}
    virtual void dosomething();
    // virtual void canActorPassThroughMe() const;
private:
    bool stat = true;
};

class Squirt : public Actor
{
public:
    Squirt(int startX, int startY, Direction startDir, StudentWorld *world) : Actor(IID_WATER_SPURT, startX, startY, startDir, 1.0, 1, world)
    {
        setVisible(true);
        setAlive();
        setDistance(0);
    }

    virtual ~Squirt() {}
    virtual void dosomething();
    virtual void incrementDistance();
    virtual void setDistance(int value);

private:
    int distance;
};
class Gold : public ActivatingObject
{
public:
    Gold(int startX, int startY, StudentWorld *world, bool temporary) : ActivatingObject(IID_GOLD, startX, startY, right, 1.0, 2, world, SOUND_GOT_GOODIE, true, true, false)
    {
        setVisible(true);
        setPickup(true);
        setAlive();
        setTicksToLive(300);
        setTemporary(temporary);
    }
    virtual ~Gold() {}
    virtual void dosomething();
};

class SonarKit : public ActivatingObject
{
public:
    SonarKit(int startX, int StartY, StudentWorld *world, int startTicks) : ActivatingObject(IID_SONAR, startX, StartY, right, 1.0, 2, world, SOUND_GOT_GOODIE, true, false, true)
    {
        setVisible(true);
        setAlive();
        setTemporary(true);
        setTicksToLive(startTicks);
    }
    virtual ~SonarKit() {}
    virtual void dosomething();
};

class WaterPool : public ActivatingObject
{
public:
    WaterPool(int startX, int startY, StudentWorld *world, int startTicks) : ActivatingObject(IID_WATER_POOL, startX, startY, right, 1.0, 2, world, SOUND_GOT_GOODIE, true, false, false)
    {
        setVisible(true);
        setAlive();
        setTemporary(true);
        setTicksToLive(startTicks);
    }

    virtual ~WaterPool() {}
    virtual void dosomething();
};

class Protester : public Agent
{
public:
    Protester(int startX, int startY, int imageID, StudentWorld *world);
    void virtual dosomething();
    void setTicksToNextMove(int ticks);
    void setProtesterState(int states);

private:
    int Protester_Ticks;
    int protester_states; // 0 is regualar state, 1 is resting state, 2 is return to base
    int stepsToTake;
};

class Regular_Protester : public Protester
{
public:
    Regular_Protester(StudentWorld *World) : Protester(IID_PROTESTER, 60, 60, World)
    {
        setVisible(true);
        setHealth(5);
        setProtesterState(0);
    }
    virtual ~Regular_Protester() {}
    virtual void dosomething();

private:
};
#endif // ACTOR_H_
