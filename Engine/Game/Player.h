#pragma once
#include "..\..\Structs\Vector3.h"
#include "..\..\Utils\ReadHelper.cpp"
#include "..\..\Utils\Offsets.h"
#include <iostream>

namespace Game
{
	class Player
	{
	private:
		uintptr_t _base;

		Vector4 *Position;
		int LocationID;
		int WorldID;
		int GBucks;

		// Story
		int Act;
		int Tier;
		int CurrentQuest;
	public:
		Player();
		Player(uintptr_t base);

		// Updaters
		bool UpdateLocation();
		bool UpdatePosition();
		bool UpdateGBucks();
		bool UpdateStoryProgress();

		// Getters
		Vector4* getPosition();
		int getLocationID();
		int getWorldID();
		int getGBucks();
		int getAct();
		int getTier();
		int getCurrentQuest();
	};
}


