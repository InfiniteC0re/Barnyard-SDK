#include "Vector4.h"

Vector4 GetVector4FromVector3(Vector3 vec3)
{
	Vector4 vec4;
	vec4.x = vec3.x;
	vec4.y = vec3.y;
	vec4.z = vec3.z;

	return vec4;
}

Vector4 GetUpVector()
{
	Vector4 vec4;
	vec4.y = -1;

	return vec4;
}