#include "APlayerManager.h"

typedef AUnitPlayer* (__thiscall* _GetCurPlayer)(APlayerManager* pThis, int index);
_GetCurPlayer N_GetCurPlayer = (_GetCurPlayer)0x0061faf0;

AUnitPlayer* APlayerManager::GetCurPlayer(int index)
{
	return N_GetCurPlayer(this, index);
}

APlayerManager** g_APlayerManager = (APlayerManager**)0x007b48a8;