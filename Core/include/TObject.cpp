#include "TObject.h"

typedef void* (__thiscall* _FindAttachedToshiClass)(TObject*, const TObjectInfo*);
_FindAttachedToshiClass N_FindAttachedToshiClass = (_FindAttachedToshiClass)0x00413cd0;

void* TObject::FindAttachedToshiClass(TObjectInfo* pClassInfo)
{
    return N_FindAttachedToshiClass(this, pClassInfo);
}

typedef TObjectInfo* (__stdcall* _FindTClassByName)(const char*, TObjectInfo*);
_FindTClassByName N_FindTClassByName = (_FindTClassByName)0x006bbfc0;

TObjectInfo* TObject::Find(const char* name, TObjectInfo* pClassInfo)
{
    return N_FindTClassByName(name, pClassInfo);
}
