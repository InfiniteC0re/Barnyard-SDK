#pragma once
#include "Vector4.h"
#include "ParticleSystem.h"

struct ParticleSystem
{
	int field_0x0;
	int field_0x4;
	ParticleSystem* prevSystem;
	int field_0xc;
	int field_0x10;
	int field_0x14;
	int field_0x18;
	int field_0x1c;
	int field_0x20;
	int field_0x24;
	int field_0x28;
	int field_0x2c;
	Vector4 coordinates;
	int field_0x40;
	int destructFunction;
	ParticleSystem* rootSystem;
};