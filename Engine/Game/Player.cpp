#include "Player.h"
#include <iostream>

Game::Player::Player() {}

Game::Player::Player(uintptr_t base)
{
    _base = base;
    LocationID = -1;
    WorldID = -1;
    GBucks = 0;

    // Story Line
    Act = -1;
    Tier = -1;
    CurrentQuest = 0;
}

// Updaters

bool Game::Player::UpdateLocation()
{
    int readedWorld = ReadHelper::ReadAddress(_base, Offsets::Player::World, 1);
    int readedLocation = -1;

    if (readedWorld == 0)
        readedLocation = ReadHelper::ReadAddress(_base, Offsets::Player::Location, 8);

    if ((readedLocation != LocationID && readedLocation != 1) || readedWorld != WorldID)
    {
        WorldID = readedWorld;
        if (readedLocation != -1) LocationID = readedLocation;

        return TRUE;
    }

    return FALSE;
}

bool Game::Player::UpdatePosition()
{
    Position = ReadHelper::ReadVector4(_base, Offsets::Player::Position, 8);

    return FALSE;
}

bool Game::Player::UpdateGBucks()
{
    int newBucks = ReadHelper::ReadAddress(_base, Offsets::Player::GBucks, 2);

    if (newBucks != GBucks)
    {
        GBucks = newBucks;

        return TRUE;
    }

    return FALSE;
}

bool Game::Player::UpdateStoryProgress()
{
    int newAct = ReadHelper::ReadAddress(_base, Offsets::Story::Act, 7);
    int newTier = ReadHelper::ReadAddress(_base, Offsets::Story::Tier, 7) - 1;

    if (newTier != Tier || newAct != Act)
    {
        if (newAct == 2)
            CurrentQuest = 15;
        else if (newAct == 3)
            CurrentQuest = 34;
        else
            CurrentQuest = 0;

        Act = newAct;
        Tier = newTier;

        CurrentQuest += Tier;

        return TRUE;
    }

    return FALSE;
}

// Getters

Vector4* Game::Player::getPosition()
{
    return Position;
}

int Game::Player::getLocationID()
{
    return LocationID;
}

int Game::Player::getWorldID()
{
    return WorldID;
}

int Game::Player::getGBucks()
{
    return GBucks;
}

int Game::Player::getAct()
{
    return Act;
}

int Game::Player::getTier()
{
    return Tier;
}

int Game::Player::getCurrentQuest()
{
    return CurrentQuest;
}