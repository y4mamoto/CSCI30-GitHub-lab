#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

using namespace std;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
};

class Person : public Actor
{
};
class object : public Actor
{
};

class Protester : public Person
{
};

class Iceman : public Person
{
};

class regularProtester : public Protester
{
};

class hardcoreProtester : public Protester
{
};

class waterRefill : public object
{
};

class oil : public object
{
};

class gold : public object
{
};

class sonar : public object
{
};

class Ice : public Actor
{
};

class Boulder : public Actor
{
};

#endif // ACTOR_H_
