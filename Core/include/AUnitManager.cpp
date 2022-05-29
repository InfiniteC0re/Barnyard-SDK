#include "AUnitManager.h"

typedef void (__thiscall* _SetUnitTransform)(UnitTransform*, Vector4* posVec, Vector4* rotVec, int x, int y, int z, bool someFlag);
_SetUnitTransform N_SetUnitTransform = (_SetUnitTransform)(0x00412ee0);

void AUnitManager::SetUnitTransform(UnitTransform* pUnitPos, Vector4* posVec, Vector4* rotVec, int x, int y, int z, bool someFlag)
{
	N_SetUnitTransform(pUnitPos, posVec, rotVec, x, y, z, someFlag);
}

typedef ToshiClass* (__thiscall* _CreateSteerByName)(AUnitManager*, TPString8**, UnitTransform*);
_CreateSteerByName N_CreateSteerByName = (_CreateSteerByName)(0x00411a30);

ToshiClass* AUnitManager::CreateSteerByName(TPString8** pName, UnitTransform* unitPos)
{
	return N_CreateSteerByName(this, pName, unitPos);
}

AUnitManager** g_AUnitManager = (AUnitManager**)0x0077e344;