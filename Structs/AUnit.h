#pragma once
#include "Vector4.h"

struct AUnitBase {
    char field_0x0;
    char field_0x1;
    char field_0x2;
    char field_0x3;
    char field_0x4;
    char field_0x5;
    char field_0x6;
    char field_0x7;
    char field_0x8;
    char field_0x9;
    char field_0xa;
    char field_0xb;
    char field_0xc;
    char field_0xd;
    char field_0xe;
    char field_0xf;
    char field_0x10;
    char field_0x11;
    char field_0x12;
    char field_0x13;
    char field_0x14;
    char field_0x15;
    char field_0x16;
    char field_0x17;
    char field_0x18;
    char field_0x19;
    char field_0x1a;
    char field_0x1b;
    char field_0x1c;
    char field_0x1d;
    char field_0x1e;
    char field_0x1f;
    char field_0x20;
    char field_0x21;
    char field_0x22;
    char field_0x23;
    Vector4* (*GetCoordinatesOfUnit)(struct AUnit*);
    char field_0x28;
    char field_0x29;
    char field_0x2a;
    char field_0x2b;
    char field_0x2c;
    char field_0x2d;
    char field_0x2e;
    char field_0x2f;
    char field_0x30;
    char field_0x31;
    char field_0x32;
    char field_0x33;
    char field_0x34;
    char field_0x35;
    char field_0x36;
    char field_0x37;
    char field_0x38;
    char field_0x39;
    char field_0x3a;
    char field_0x3b;
    char field_0x3c;
    char field_0x3d;
    char field_0x3e;
    char field_0x3f;
    char field_0x40;
    char field_0x41;
    char field_0x42;
    char field_0x43;
    char field_0x44;
    char field_0x45;
    char field_0x46;
    char field_0x47;
    char field_0x48;
    char field_0x49;
    char field_0x4a;
    char field_0x4b;
    char field_0x4c;
    char field_0x4d;
    char field_0x4e;
    char field_0x4f;
    char field_0x50;
    char field_0x51;
    char field_0x52;
    char field_0x53;
    char field_0x54;
    char field_0x55;
    char field_0x56;
    char field_0x57;
    char field_0x58;
    char field_0x59;
    char field_0x5a;
    char field_0x5b;
    char field_0x5c;
    char field_0x5d;
    char field_0x5e;
    char field_0x5f;
    char field_0x60;
    char field_0x61;
    char field_0x62;
    char field_0x63;
    char field_0x64;
    char field_0x65;
    char field_0x66;
    char field_0x67;
    char field_0x68;
    char field_0x69;
    char field_0x6a;
    char field_0x6b;
    char field_0x6c;
    char field_0x6d;
    char field_0x6e;
    char field_0x6f;
    char field_0x70;
    char field_0x71;
    char field_0x72;
    char field_0x73;
    void (*applyGravity)(AUnit*, float);
};

struct AUnit {
    AUnitBase* pBase;
    char field_0x4;
    char field_0x5;
    char field_0x6;
    char field_0x7;
    char field_0x8;
    char field_0x9;
    char field_0xa;
    char field_0xb;
    char field_0xc;
    char field_0xd;
    char field_0xe;
    char field_0xf;
    char field_0x10;
    char field_0x11;
    char field_0x12;
    char field_0x13;
    Vector4 coordinates;
    float fallSpeed;
    float gravityStrength;
    char field_0x30;
    char field_0x31;
    char field_0x32;
    char field_0x33;
    char field_0x34;
    char field_0x35;
    char field_0x36;
    char field_0x37;
    char field_0x38;
    char field_0x39;
    char field_0x3a;
    char field_0x3b;
    char field_0x3c;
    char field_0x3d;
    char field_0x3e;
    char field_0x3f;
    float speed1;
    float speed2;
    float targetSpeed;
    char field_0x4c;
    char field_0x4d;
    char field_0x4e;
    char field_0x4f;
    char field_0x50;
    char field_0x51;
    char field_0x52;
    char field_0x53;
    char field_0x54;
    char field_0x55;
    char field_0x56;
    char field_0x57;
    char field_0x58;
    char field_0x59;
    char field_0x5a;
    char field_0x5b;
    char field_0x5c;
    char field_0x5d;
    char field_0x5e;
    char field_0x5f;
    char field_0x60;
    char field_0x61;
    char field_0x62;
    char field_0x63;
    char field_0x64;
    char field_0x65;
    char field_0x66;
    char field_0x67;
    char field_0x68;
    char field_0x69;
    char field_0x6a;
    char field_0x6b;
    float timeInAir;
    char field_0x70;
    char field_0x71;
    char field_0x72;
    char field_0x73;
    char field_0x74;
};

