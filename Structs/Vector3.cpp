#include "Vector3.h"

Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(float xCoord, float yCoord, float zCoord)
{
	x = xCoord;
	y = yCoord;
	z = zCoord;
}

void Vector3::Set(float xCoord, float yCoord, float zCoord)
{
	x = xCoord;
	y = yCoord;
	z = zCoord;
}

bool Vector3::isNull()
{
	return x == 0 && y == 0 && z == 0;
}
