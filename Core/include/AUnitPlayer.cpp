#include "AUnitPlayer.h"

typedef void (__thiscall* _SetSkin)(AUnitPlayer*, int, int);
_SetSkin N_SetSkin = (_SetSkin)0x00415b10;

void AUnitPlayer::SetSkin(int index)
{
	N_SetSkin(this, index, 1);
}