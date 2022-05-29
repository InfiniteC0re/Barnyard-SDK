#include "TPString8.h"

typedef void* (__stdcall* _GetEmpty)();
_GetEmpty N_GetEmpty = (_GetEmpty)(0x006bb580);

static void* GetEmptyStruct()
{
	return N_GetEmpty();
}

typedef TPString8* (__thiscall* _GetTPString8)(void* pThis, TPString8** pOut, const char* text, char* unk);
_GetTPString8 N_GetTPString8 = (_GetTPString8)(0x006bc3e0);

TPString8* TPString8::Find(const char* name)
{
	void* empty = GetEmptyStruct();
	TPString8* pStr = nullptr;
	
	N_GetTPString8(empty, &pStr, name, (char*)0x0);
	return pStr;
}
