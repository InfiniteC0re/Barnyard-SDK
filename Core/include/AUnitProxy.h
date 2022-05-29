#pragma once
#include "ToshiClass.h"

class AUnitPlayer;

class AUnitProxy : public ToshiClass
{
public:
    void* m_vTable;
    int field_0x4;
    int field_0x8;
    int field_0xc;
    int field_0x10;
    AUnitPlayer* pPlayer;
    int field_0x18;
    int field_0x1c;
};

