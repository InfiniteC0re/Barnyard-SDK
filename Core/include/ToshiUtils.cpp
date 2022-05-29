#include "ToshiUtils.h"

typedef int* (__stdcall* _TAlloc)(unsigned int size, void* block, int flag1, int flag2);
_TAlloc N_TAlloc = (_TAlloc)0x006b5540;

int* ToshiUtils::TAlloc(int size, void* block)
{
    return N_TAlloc(size, block, 0, 0xffffffff);
}

typedef bool (__stdcall* _TFree)(void*);
_TFree N_TFree = (_TFree)0x006b4a20;

bool ToshiUtils::TFree(void* object)
{
    return N_TFree(object);
}

typedef void (__stdcall* _MakeVector)(void* param_1, int structSize, int count, int structMaker, int param_5);
_MakeVector N_MakeVector = (_MakeVector)0x006de7f2;

void ToshiUtils::MakeVector(void* param_1, int structSize, int count, int structMaker, int param_5)
{
    N_MakeVector(param_1, structSize, count, structMaker, param_5);
}

typedef void (__thiscall* _CopyString)(void* dest, char* src);
_CopyString N_CopyString = (_CopyString)0x006bd6a0;

void ToshiUtils::CopyString(void* dest, char* src)
{
    N_CopyString(dest, src);
}

typedef void(__thiscall* _CloseTRB)(void* trb);
_CloseTRB N_CloseTRB = (_CloseTRB)0x006baa20;

void ToshiUtils::CloseTRB(void* trb)
{
    N_CloseTRB(trb);
}

typedef void*(__thiscall* _DoesTextureExist)(void* pThis, const char* name, int f1, int f2);
_DoesTextureExist N_DoesTextureExist = (_DoesTextureExist)0x00615b40;

void* ToshiUtils::DoesTextureExist(const char* name, int f1, int f2)
{
    return N_DoesTextureExist(*(void**)0x007b46c8, name, f1, f2);
}

typedef int (__thiscall* _SaveDDSTexture)(DDSTextureData* pTextureData);
_SaveDDSTexture N_SaveDDSTexture = (_SaveDDSTexture)0x00615bc0;

int ToshiUtils::SaveDDSTexture(DDSTextureData* pTextureData)
{
    return N_SaveDDSTexture(pTextureData);
}
