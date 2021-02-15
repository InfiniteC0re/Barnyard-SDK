#pragma once

class Vector3
{
public:
	Vector3();
	Vector3(float xCoord, float yCoord, float zCoord);

	float x;
	float y;
	float z;

	void Set(float xCoord, float yCoord, float zCoord);
	bool isNull();
};

