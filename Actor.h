#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

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

    virtual void dosomething() = 0;

private:
    StudentWorld *world;
};

class Person : public Actor
{
public:
    Person(int imageID, int startX, int startY, Direction startDirection, float size, unsigned int depth, StudentWorld *newWorld) : Actor(imageID, startX, startY, startDirection, size, depth, newWorld)
    {
        setVisible(true);
    }

    virtual void setHealth(int health);
    virtual int getHealth();
    virtual void damage(int val);

private:
    int num_health;
};

class Iceman : public Person
{
public:
    Iceman(int startX, int startY, StudentWorld *World) : Person(IID_PLAYER, startX, startY, right, 1.0, 0, World)
    {
        setVisible(true);
    }
    virtual void dosomething();
    virtual void giveup();
    virtual bool aliveCheck();

private:
    bool alive;
};

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