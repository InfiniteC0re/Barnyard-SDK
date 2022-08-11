#pragma once
#include "Units/AUnitPlayer.h"
#include "TObject.h"

class APlayerManager : public TObject
{
public:
	AUnitPlayer* GetCurPlayer(int index);
};

extern APlayerManager** g_APlayerManager;
