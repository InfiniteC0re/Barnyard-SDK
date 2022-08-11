#include "AUnitManager.h"

typedef AUnit* (__thiscall* _CreateUnitByName)(AUnitManager*, TPString8**, Mat43*);
_CreateUnitByName N_CreateUnitByName = (_CreateUnitByName)(0x00411a30);

AUnit* AUnitManager::CreateUnitByName(TPString8** pName, Mat43* transform)
{
	return N_CreateUnitByName(this, pName, transform);
}

AUnitManager** g_AUnitManager = (AUnitManager**)0x0077e344;