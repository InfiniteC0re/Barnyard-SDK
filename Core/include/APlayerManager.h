#pragma once
#include "AUnitPlayer.h"
#include "ToshiClass.h"

class APlayerManager : public ToshiClass
{
public:
	AUnitPlayer* GetCurPlayer(int index);
};

extern APlayerManager** g_APlayerManager;
