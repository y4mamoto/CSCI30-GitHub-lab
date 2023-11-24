#include "Actor.h"
#include "StudentWorld.h"

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

void Person::setHealth(int health)
{ // Set the health
    num_health = health;
}

int Person::getHealth()
{
    return num_health;
}

void Person::damage(int val)
{ // Character take damage
    num_health -= val;
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

bool Person::aliveCheck()
{
    return alive;
}

bool Person::deadCheck()
{
    return dead;
}

void Person::setDead()
{
    alive = false;
    dead = true;
}
