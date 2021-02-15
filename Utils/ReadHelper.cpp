#pragma once
#include <Windows.h>
#include <iostream>
#include <string>

#include "../Structs/Vector3.h"

class ReadHelper
{
public:
	static std::string ReadString(int offset)
	{
		std::string str = "";
		int prevByte = NULL;

		while (true)
		{
			const char byte = (char)(*(uintptr_t*)((int)offset++));

			if (byte == NULL && prevByte == NULL)
				return str;

			if (byte != NULL)
				str += byte;
			prevByte = byte;
		}
	}

	static uintptr_t ReadAddress(uintptr_t addr, const uintptr_t offsets[], int size)
	{
		for (int i = 0; i < size; i++)
		{
			if (addr + offsets[i] >= 0xFFFFFFF) return NULL;
			if (addr != NULL) addr = *(uintptr_t*)(addr + offsets[i]);
			if (addr >= 0xFFFFFFF) return NULL;
		}

		return addr;
	}

	static Vector3 ReadVector3(uintptr_t addr, const uintptr_t offsets[], int size) {
		Vector3 vec;

		for (int i = 0; i < size - 1; i++)
		{
			if (addr + offsets[i] >= 0xFFFFFFF) return vec;
			if (addr != NULL) addr = *(uintptr_t*)(addr + offsets[i]);
			if (addr >= 0xFFFFFFF) return vec;
		}

		if (addr != NULL)
		{
			vec.x = *(float*)(addr + offsets[size - 1]);
			vec.y = *(float*)(addr + offsets[size - 1] + 0x04);
			vec.z = *(float*)(addr + offsets[size - 1] + 0x08);
		}

		return vec;
	}
};