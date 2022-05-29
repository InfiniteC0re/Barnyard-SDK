#pragma once
#include "ToshiClass.h"

enum AInteractionSequenceFlags {
    EMPTY = 0,
    NOT_EXECUTE = 8,
    UNK1 = 1,
    UNK2 = 2,
    UNK3 = 4
};

class AInteractionSequence : public ToshiClass
{
public:
    void* field_0x0;
    int field_0x4;
    void* field_0x8;
    void* field_0xc;
    int field_0x10;
    int field_0x14;
    int field_0x18;
    int field_0x1c;
    int field_0x20;
    int field_0x24;
    int field_0x28;
    int field_0x2c;
    int field_0x30;
    int field_0x34;
    int field_0x38;
    void* field_0x3c;
    int field_0x40;
    int field_0x44;
    int field_0x48;
    int field_0x4c;
    int field_0x50;
    int m_iFlag;
    int field_0x58;
    int field_0x5c;
    int field_0x60;
    int field_0x64;
    int field_0x68;
    int field_0x6c;
    int field_0x70;
};

