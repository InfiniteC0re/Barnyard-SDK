#pragma once
#include "TObject.h"
#include "TPString8.h"
#include "Math.h"
#include "Units/AUnit.h"

class AUnitPlayer;

class AUnitManager : public TObject
{
public:
	int unk1;
	int unk2;
	int unk3;
	int unk4;
	int unk5;
	int unk6;
	int unk7;
	int unk8;
	int unk9;
	int unk10;
	int unk11;
	int unk12;
	int unk13;
	int unk14;
	AUnitPlayer* m_pPlayer;
	
	AUnit* CreateUnitByName(TPString8** pName, Mat43* transform);
};

extern AUnitManager** g_AUnitManager;

