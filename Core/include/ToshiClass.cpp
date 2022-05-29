#include "ToshiClass.h"


ToshiClassVTable* ToshiClass::GetVTable()
{
    return *(ToshiClassVTable**)this;
}

typedef void* (__thiscall* _FindAttachedToshiClass)(ToshiClass*, const ToshiClassInfo*);
_FindAttachedToshiClass N_FindAttachedToshiClass = (_FindAttachedToshiClass)0x00413cd0;

void* ToshiClass::FindAttachedToshiClass(ToshiClassInfo* pClassInfo)
{
    return N_FindAttachedToshiClass(this, pClassInfo);
}

typedef ToshiClassInfo* (__stdcall* _FindTClassByName)(const char*, ToshiClassInfo*);
_FindTClassByName N_FindTClassByName = (_FindTClassByName)0x006bbfc0;

ToshiClassInfo* ToshiClass::FindByName(const char* name, ToshiClassInfo* pClassInfo)
{
    return N_FindTClassByName(name, pClassInfo);
}
