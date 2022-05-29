#pragma once
#include "ToshiClass.h"
#include "AGTAStyleCameraHelper.h"

class ACameraManager : public ToshiClass
{
public:
    void* field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xc;
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
    int field_0x3c;
    int field_0x40;
    int field_0x44;
    int field_0x48;
    int field_0x4c;
    int field_0x50;
    int field_0x54;
    struct S5* pS5;
    int field_0x5c;
    int field_0x60;
    int field_0x64;
    AGTAStyleCameraHelper* m_pCameraHelper;
    int field_0x6c;
    int field_0x70;
    int field_0x74;
    int field_0x78;
    int field_0x7c;
    int field_0x80;
    int field_0x84;
    int* m_pCamera;
    int field_0x8c;
    int field_0x90;
    int field_0x94;
    int field_0x98;
    int field_0x9c;
    int field_0xa0;
    int field_0xa4;
    int field_0xa8;
    int field_0xac;
    int field_0xb0;
    int field_0xb4;
    int field_0xb8;
    int field_0xbc;
    int field_0xc0;
    int field_0xc4;
    int field_0xc8;
    int field_0xcc;
    int field_0xd0;
    int field_0xd4;
    int field_0xd8;
    int field_0xdc;
    int field_0xe0;
};

extern ACameraManager** g_ACameraManager;