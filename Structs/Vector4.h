#pragma once
#include "Vector3.h"

struct Vector4
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;
};

Vector4 GetVector4FromVector3(Vector3 vec3);
Vector4 GetUpVector();
