#pragma once
#include <Windows.h>

namespace Offsets
{
	namespace Player
	{
		constexpr uintptr_t Location[] = { 0x003B470C, 0x19C, 0x404, 0x0, 0x0, 0x14, 0x0, 0x32C };
		constexpr uintptr_t Position[] = { 0x0038A488, 0x68, 0x38, 0x54, 0x3C, 0x1C, 0x20, 0x18 };
		constexpr uintptr_t GBucks[] = { 0x003B493C, 0x1C };
		constexpr uintptr_t World[] = { 0x372800 };
	};

	namespace Story
	{
		constexpr uintptr_t Tier[] = { 0x003B4B5C, 0x14, 0x8C, 0x14, 0x60, 0x10, 0xAE8 };
		constexpr uintptr_t Act[] = { 0x003B4B5C, 0x14, 0x8C, 0x14, 0x60, 0x10, 0xAEC };
	}
	
	namespace Misc {
		constexpr uintptr_t FPSValue[] = { 0x00339A88 };
		constexpr uintptr_t ViewMatrix[] = { 0x0038DEFC };
		constexpr uintptr_t JumpFromFPS[] = { 0x0023588F };
	}
}

