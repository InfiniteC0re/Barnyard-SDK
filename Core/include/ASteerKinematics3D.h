#pragma once
#include "TObject.h"
#include "Math.h"

class ASteerKinematics3D : public TObject
{
public:
    int* field_0x4;
    int* field_0x8;
    int* field_0xc;
    int* field_0x10;
    unsigned int m_iBehaviourFlag;
    Vector4 m_vPosition;
    float m_fLocalVelocityY;
    float m_fGravityScale;
    Vector4 m_qLastRotation;
    float m_fRealVelocity;
    float m_fAnimForwardVelocity;
    float m_fTargetForwardVelocity;
    Vector4 m_qRotation;
    float field_0x5c;
    float field_0x60;
    float field_0x64;
    float field_0x68;
    float m_fGlobalVelocityY;
    Vector4 field_0x70;
    Vector4 field_0x80;
    Vector4 field_0x90;
    Vector4 field_0xa0;
    Vector4 vec4;
};

