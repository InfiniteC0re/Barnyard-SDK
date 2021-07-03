#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include "../Structs/Vector3.h"
#include "../Structs/Vector4.h"

class ReadHelper
{
public:
	static std::string ReadString(int offset)
	{
		std::string str = "";

		while (true)
		{
			const char byte = *(char*)offset++;

			if (byte == NULL)
				return str;

			if (byte != NULL)
				str += byte;
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

	static int* ReadPointer(int addr, const uintptr_t offsets[], int size)
	{
		for (int i = 0; i < size; i++)
		{
			if (addr + offsets[i] >= 0xFFFFFFF) return NULL;
			if (addr != NULL) addr = *(int*)(addr + offsets[i]);
			if (addr >= 0xFFFFFFF) return NULL;
		}

		return &addr;
	}

	static Vector3* ReadVector3(uintptr_t addr, const uintptr_t offsets[], int size) {
		Vector3* vec = (Vector3*)0x0;

		for (int i = 0; i < size - 1; i++)
		{
			if (addr + offsets[i] >= 0xFFFFFFF) return vec;
			if (addr != NULL) addr = *(uintptr_t*)(addr + offsets[i]);
			if (addr >= 0xFFFFFFF) return vec;
		}

		if (addr != NULL)
			vec = reinterpret_cast<Vector3*>(addr + 0x18);

		return vec;
	}

	static Vector4* ReadVector4(uintptr_t addr, const uintptr_t offsets[], int size) {
		Vector4* vec = (Vector4*)0x0;

		for (int i = 0; i < size - 1; i++)
		{
			if (addr + offsets[i] >= 0xFFFFFFF) return vec;
			if (addr != NULL) addr = *(uintptr_t*)(addr + offsets[i]);
			if (addr >= 0xFFFFFFF) return vec;
		}

		if (addr != NULL)
			vec = reinterpret_cast<Vector4*>(addr + 0x18);

		return vec;
	}
};