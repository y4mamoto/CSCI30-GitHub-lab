#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"
#include "GameWorld.h"
#include <algorithm>
#include <utility>
#include <queue>

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

bool ActivatingObject::isPickable()
{
    return pickable;
}

void ActivatingObject::setTicksToLive(int value)
{
    ticks = value;
}

int ActivatingObject::getTicks() const
{
    return ticks;
}

void ActivatingObject::removeTicks()
{
    ticks--;
}

bool ActivatingObject::isTemporary()
{
    return temp;
}

void ActivatingObject::setTemporary(bool temporary)
{
    temp = temporary;
}

void Iceman::dosomething()
{
    if (deadCheck()) // Check if iceman is dead
        return;

    if (getHealth() == 0)
    {
        setDead();
    }

    getWorld()->removeIce(getX(), getY()); // Remove the ice

    int ch;
    if (getWorld()->getKey(ch) == true)
    { // Movement from user input
        switch (ch)
        {
        case KEY_PRESS_LEFT:
            if ((getWorld()->boulderCheck(getX(), getY(), left)))
            {
                moveTo(getX(), getY());
                break;
            }
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
            if ((getWorld()->boulderCheck(getX(), getY(), right)))
            {
                moveTo(getX(), getY());
                break;
            }
            if (getDirection() != right)
                setDirection(right);
            else if (getX() <= 59)
                moveTo(getX() + 1, getY());
            else if (getX() == 60)
                moveTo(getX(), getY());
            break;

        case KEY_PRESS_UP:
            if (getWorld()->boulderCheck(getX(), getY(), up))
            {
                moveTo(getX(), getY());
                break;
            }
            if (getDirection() != up)
                setDirection(up);
            else if (getY() <= 59)
                moveTo(getX(), getY() + 1);
            else if (getY() == 60)
                moveTo(getX(), getY());
            break;

        case KEY_PRESS_DOWN:
            if (getWorld()->boulderCheck(getX(), getY() - 3, down))
            {
                moveTo(getX(), getY());
                break;
            }
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
        case KEY_PRESS_TAB:
            if (gold > 0)
            {
                gold--;
                getWorld()->goldDrop(getX(), getY());
            }
            break;
        case 'z':
        case 'Z':
            if (sonar > 0)
            {
                sonar--;
                getWorld()->revealAllNearbyObjects(getX(), getY(), 12);
                break;
            }
        case KEY_PRESS_SPACE:
            Direction dir;
            dir = getDirection();
            if (water > 0)
            {
                switch (dir)
                {
                case GraphObject::right:
                    getWorld()->generateSquirt(getX() + 3, getY(), right);
                    water--;
                    GameController::getInstance().playSound(SOUND_PLAYER_SQUIRT);
                    break;

                case GraphObject::left:
                    getWorld()->generateSquirt(getX() - 3, getY(), left);
                    water--;
                    GameController::getInstance().playSound(SOUND_PLAYER_SQUIRT);
                    break;

                case GraphObject::up:
                    getWorld()->generateSquirt(getX(), getY() + 3, up);
                    water--;
                    GameController::getInstance().playSound(SOUND_PLAYER_SQUIRT);
                    break;

                case GraphObject::down:
                    getWorld()->generateSquirt(getX(), getY() - 3, down);
                    water--;
                    GameController::getInstance().playSound(SOUND_PLAYER_SQUIRT);
                    break;
                default:
                    break;
                }
            }
        }
    }
}

unsigned int Iceman::getGold() const
{
    return gold;
}

void Iceman::addGold()
{
    gold++;
}

unsigned int Iceman::getSonar() const
{
    return sonar;
}

void Iceman::addSonar()
{
    sonar++;
}

unsigned int Iceman::getWater() const
{
    return water;
}

void Iceman::addWater()
{
    water += 5;
}

void Squirt::dosomething()
{

    Direction dir;
    dir = getDirection();

    if (distance == 4)
        setDead();

    if ((getWorld()->boulderCheck(getX(), getY(), dir)) || (getWorld()->IceCheck(getX(), getY(), dir)))
        setDead();

    if (getX() >= 63 || getX() <= 0 || getY() >= 63 || getY() <= 0)
    {
        return;
    }

    switch (dir)
    {
    case GraphObject::right:

        moveTo(getX() + 1, getY());
        incrementDistance();

        break;
    case GraphObject::left:
        moveTo(getX() - 1, getY());
        incrementDistance();
        break;
    case GraphObject::down:
        moveTo(getX(), getY() - 1);
        incrementDistance();
        break;
    case GraphObject::up:
        moveTo(getX(), getY() + 1);
        incrementDistance();
        break;
    default:
        break;
    }
}

void Squirt::setDistance(int value)
{
    distance = value;
}

void Squirt::incrementDistance()
{
    distance++;
}

void Boulder::dosomething()
{
    if (deadCheck())
    {
        return;
    }
    if (!floorIceCheck())
    {
        setState(1);
    }

    if (boulder_states == 1 && boulder_ticks < 30)
    {
        boulder_ticks++;
    }

    if (boulder_states == 1 && boulder_ticks >= 30)
    {
        setState(2);
        GameController::getInstance().playSound(SOUND_FALLING_ROCK);
    }

    if (boulder_states == 2)
    {
        if (getWorld()->nearIcemanCheck(getX(), getY(), 3))
        {
            getWorld()->boulderDamage(getX(), getY());
            setDead();
        }
        if (getWorld()->IceCheck(getX(), getY() - 1, down) == true)
        {
            setDead();
            return;
        }
        moveTo(getX(), getY() - 1);
    }
}

void Boulder::setState(int stateOfBoulder)
{
    boulder_states = stateOfBoulder;
}

bool Boulder::floorIceCheck()
{
    for (int i = 0; i < 4; i++)
    {
        if (getWorld()->IceCheck(getX() + i, getY(), down))
        {
            return true;
        }
    }
    return false;
}

void BarrelOfOil::dosomething()
{
    if (deadCheck())
        return;

    if (getWorld()->nearIcemanCheck(getX(), getY(), 4))
    {
        setVisible(true);
    }
    if (getWorld()->nearIcemanCheck(getX(), getY(), 3))
    {
        setDead();
        GameController::getInstance().playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        getWorld()->pickupBarrel();
    }
}

void Gold::dosomething()
{
    if (deadCheck())
        return;

    if (isTemporary())
    {
        setPickup(false);
        setVisible(true);
    }

    if (getWorld()->nearIcemanCheck(getX(), getY(), 4))
    {
        setVisible(true);
    }
    if (isPickable() && getWorld()->nearIcemanCheck(getX(), getY(), 3))
    {
        setDead();
        GameController::getInstance().playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(10);
        getWorld()->giveIcemanGold();
    }

    if (!isPickable())
    {
        if (getTicks() == 0)
            setDead();

        removeTicks();
    }
}

void SonarKit::dosomething()
{
    if (deadCheck())
        return;

    if (getWorld()->nearIcemanCheck(getX(), getY(), 3))
    {
        setDead();
        GameController::getInstance().playSound(SOUND_GOT_GOODIE);
        getWorld()->giveIceManSonar();
        getWorld()->increaseScore(75);
    }

    if (getTicks() == 0)
    {
        setDead();
    }

    removeTicks();
}

void WaterPool::dosomething()
{
    if (deadCheck())
        return;

    if (getWorld()->nearIcemanCheck(getX(), getY(), 3))
    {
        setDead();
        GameController::getInstance().playSound(SOUND_GOT_GOODIE);
        getWorld()->giveIceManWater();
        getWorld()->increaseScore(100);
    }
    if (getTicks() == 0)
    {
        setDead();
    }
    removeTicks();
}

void Protester::setTicksToNextMove(int ticks)
{
    Protester_Ticks = ticks;
}

void Protester::setProtesterState(int states)
{
    Protester_Ticks = states;
}

int Protester::getStates()
{
    return protester_states;
}

int Protester::getRestingTicks()
{
    return restting_ticks;
}

void Protester::decrementRestingTicks()
{
    restting_ticks--;
}

int Protester::getDistanceToTravel()
{
    return stepsToTake;
}

void Protester::decrementDistance()
{
    stepsToTake--;
}

void Protester::setWait()
{
    ticksToWaitBetweenMoves = max<int>(0, 3 - getWorld()->getLevel() / 4);
}

int Protester::getWaitMoveTicks()
{
    return ticksToWaitBetweenMoves;
}

int Protester::getWaitTicks()
{
    return waitTicks;
}

void Protester::wait()
{
    waitTicks++;
}

void Protester::resetWaitTicks()
{
    waitTicks = 0;
}

void Protester::resetMove()
{
    stepsToTake = 0;
}

void Protester::setDistanceToTravel()
{
    int setDistance = random() % 60;
    while (setDistance < 8 || setDistance > 60)
    {
        setDistance = random() % 60;
    }
    stepsToTake = setDistance;
}

bool Protester::inFrontCheck()
{
    Direction dir;
    dir = getDirection();

    switch (dir)
    {
    case GraphObject::left:
        if (getWorld()->IceCheck(getX() - 1, getY(), left) || getWorld()->boulderCheck(getX() - 1, getY(), left))
        {
            return true;
        }
        break;
    case GraphObject::right:
        if (getWorld()->IceCheck(getX() + 1, getY(), right) || getWorld()->boulderCheck(getX() + 1, getY(), right))
        {
            return true;
        }
        break;
    case GraphObject::up:
        if (getWorld()->IceCheck(getX(), getY() + 1, up) || getWorld()->boulderCheck(getX(), getY() + 1, right))
        {
            return true;
        }
        break;
    case GraphObject::down:
        if (getWorld()->IceCheck(getX(), getY() - 1, down) || getWorld()->boulderCheck(getX(), getY() - 1, left))
        {
            return true;
        }
        break;

    default:
        break;
    }
    return false;
}

void Regular_Protester::bribed()
{
    GameController::getInstance().playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(25);
    setProtesterState(2);
}

void Regular_Protester::dosomething()
{
    if (deadCheck())
    {
        return;
    }

    setWait();
    Direction dir;
    dir = getDirection();
    if (getWaitMoveTicks() != getWaitTicks())
    {
        wait();
        return;
    }

    if (getWaitTicks() == getWaitMoveTicks())
    {
        resetWaitTicks();
    }

    if (inFrontCheck() && getStates() == 0)
    {
        if (getDistanceToTravel() != 0)
        {
            resetMove();
        }
        dir = getWorld()->newDirection(getX(), getY());
        setDirection(dir);
        setDistanceToTravel();
    }

    if (getStates() == 0 && getDistanceToTravel() == 0)
    {
        setDistanceToTravel();
        dir = getWorld()->newDirection(getX(), getY());
        setDirection(dir);
    }

    if (getStates() == 1 && getRestingTicks() != 0)
    {
        decrementRestingTicks();
        return;
    }

    if (getStates() == 0 && getDistanceToTravel() != 0)
    {
        switch (dir)
        {
        case GraphObject::left:
            if (!inFrontCheck() && getX() > 0)
            {
                moveTo(getX() - 1, getY());
                decrementDistance();
            }
            break;
        case GraphObject::right:
            if (!inFrontCheck() && getX() < 60)
            {
                moveTo(getX() + 1, getY());
                decrementDistance();
            }
            break;
        case GraphObject::up:
            if (!inFrontCheck() && getY() < 60)
            {
                moveTo(getX(), getY() + 1);
                decrementDistance();
            }
            break;
        case GraphObject::down:
            if (!inFrontCheck() && getY() > 0)
            {
                moveTo(getX(), getY() - 1);
                decrementDistance();
            }
            break;

        default:
            break;
        }

        wait();
    }
}
