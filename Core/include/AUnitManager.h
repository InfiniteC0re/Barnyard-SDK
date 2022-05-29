#pragma once
#include "ToshiClass.h"
#include "TPString8.h"
#include "Vectors.h"

struct UnitTransform
{
	Vector4 vec1;
	Vector4 vec2;
	Vector4 vec3;
};

class AUnitPlayer;

class AUnitManager : public ToshiClass
{
public:
	int m_vTable;
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

	static void SetUnitTransform(UnitTransform* pUnitPos, Vector4* posVec, Vector4* rotVec, int x, int y, int z, bool someFlag);
	ToshiClass* CreateSteerByName(TPString8** pName, UnitTransform* unitPos);
};

extern AUnitManager** g_AUnitManager;

