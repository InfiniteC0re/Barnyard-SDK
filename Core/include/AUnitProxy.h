#pragma once
#include "TObject.h"

class AUnitPlayer;

class AUnitProxy : public TObject
{
public:
    int field_0x4;
    int field_0x8;
    int field_0xc;
    int field_0x10;
    AUnitPlayer* pPlayer;
    int field_0x18;
    int field_0x1c;
};

