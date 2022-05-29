#include "AAlienSteer.h"

typedef bool (__thiscall* _SetMovementSpeed)(void* pThis, Vector4*);
_SetMovementSpeed N_SetMovementSpeed = (_SetMovementSpeed)(0x00556840);

bool AAlienSteer::SetMovementSpeed(Vector4* pSpeed)
{
    return N_SetMovementSpeed(this, pSpeed);
}

typedef int(__thiscall* _Jump)(void* pThis, int flag);
_Jump N_Jump = (_Jump)(0x005567c0);

int AAlienSteer::Jump()
{
    return N_Jump(this, 3);
}
