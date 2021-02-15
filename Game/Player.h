#pragma once
#include "..\Structs\Vector3.h"
#include "..\Utils\ReadHelper.cpp"
#include "..\Utils\Offsets.h"
#include <iostream>

namespace Game
{
	class Player
	{
	private:
		uintptr_t _base;

		Vector3 Position;
		int LocationID;
		int WorldID;
		int GBucks;

		// Story
		int Act;
		int Tier;
		int CurrentQuest;
	public:
		Player(uintptr_t base);

		// Updaters
		bool UpdateLocation();
		bool UpdatePosition();
		bool UpdateGBucks();
		bool UpdateStoryProgress();

		// Getters
		Vector3 getPosition();
		int getLocationID();
		int getWorldID();
		int getGBucks();
		int getAct();
		int getTier();
		int getCurrentQuest();
	};
}


