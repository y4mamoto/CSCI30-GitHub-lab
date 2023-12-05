#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"
#include "GameWorld.h"
#include <algorithm>
#include <utility>

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
bool Actor::aliveCheck()
{
    return alive;
}

bool Actor::deadCheck()
{
    return dead;
}

void Actor::setDead()
{
    alive = false;
    dead = true;
}

void Actor::setAlive()
{
    alive = true;
    dead = false;
}

void Agent::setHealth(int health)
{ // Set the health
    num_health = health;
}

int Agent::getHealth()
{
    return num_health;
}

void Agent::damage(int val)
{ // Character take damage
    num_health -= val;
}

void ActivatingObject::setPickup(bool pickup)
{
    pickable = pickup;
}

bool ActivatingObject::isVisible()
{
    return visible;
}

bool ActivatingObject::isPickable()
{
    return pickable;
}

void Iceman::dosomething()
{
    if (deadCheck()) // Check if iceman is dead
        return;

    getWorld()->removeIce(getX(), getY()); // Remove the ice

    int ch;
    if (getWorld()->getKey(ch) == true)
    { // Movement from user input
        switch (ch)
        {
        case KEY_PRESS_LEFT:
            if (getDirection() != left)
                setDirection(left);
            else if (getX() >= 1)
            {
                moveTo(getX() - 1, getY());
            }
            else if (getX() == 0)
                moveTo(getX(), getY());
            break;

        case KEY_PRESS_RIGHT:
            if (getDirection() != right)
                setDirection(right);
            else if (getX() <= 59)
                moveTo(getX() + 1, getY());
            else if (getX() == 60)
                moveTo(getX(), getY());
            break;

        case KEY_PRESS_UP:
            if (getDirection() != up)
                setDirection(up);
            else if (getY() <= 59)
                moveTo(getX(), getY() + 1);
            else if (getY() == 60)
                moveTo(getX(), getY());
            break;

        case KEY_PRESS_DOWN:
            if (getDirection() != down)
                setDirection(down);
            else if (getY() >= 1)
                moveTo(getX(), getY() - 1);
            else if (getY() == 0)
                moveTo(getX(), getY());
            break;
        case KEY_PRESS_ESCAPE: // Press escape to give up the level
            // User giveup
            setDead();
            break;
        }
    }
}

void BarrelOfOil::dosomething()
{
    if (deadCheck())
        return;

    if (getWorld()->nearIcemanCheck(getX(), getY(), 4))
    {
        setVisible(true);
        return; // Not Running properly
    }
    if (getWorld()->nearIcemanCheck(getX(), getY(), 3))
    {
        setDead();
        GameController::getInstance().playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
    }
}
