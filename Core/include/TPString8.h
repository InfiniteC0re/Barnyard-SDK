#pragma once

class TPString8
{
public:
	int unknown = 0;
	char* pString;
	int stringLength;
	int somePointer = 0x007724C0; // PTR_FUN
	int unknown2 = 0x028E4400;
	int unknown3 = 0x0280C204;
	int unknown4 = 0x00000015;
	int unknown5 = 0x029823B0;

	static TPString8* Find(const char* name);
};