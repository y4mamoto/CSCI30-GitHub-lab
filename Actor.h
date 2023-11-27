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

    virtual void dosomething() = 0;

private:
    StudentWorld *world;
};

// Person class for anything that can move
class Person : public Actor
{
public:
    Person(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, StudentWorld *newWorld) : Actor(imageID, startX, startY, startDirection, size, depth, newWorld)
    {
        setVisible(true);
        alive = true;
        dead = false;
    }

    virtual void setHealth(int health);
    virtual int getHealth();
    virtual void damage(int val);
    virtual bool aliveCheck();
    virtual bool deadCheck();
    virtual void setDead();

private:
    int num_health;
    bool alive;
    bool dead;
};

// Iceman class, the player
class Iceman : public Person
{
public:
    Iceman(int startX, int startY, StudentWorld *World) : Person(IID_PLAYER, startX, startY, right, 1.0, 0, World)
    {
        setVisible(true);
        setHealth(10);
    }
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

    virtual ~Ice(){};

    virtual void dosomething() {} // does nothing
};

#endif // ACTOR_H_
