#pragma once
#include <Windows.h>
#include <string>

namespace Engine {
	class ALocalization
	{
	private:
		uintptr_t _base;
		typedef signed int (__thiscall* _GetString)(void* pThis, int stringOffset);
		_GetString GetStringA;
	public:
		std::string GetString(int stringOffset);
		ALocalization(uintptr_t base);
	};
}
