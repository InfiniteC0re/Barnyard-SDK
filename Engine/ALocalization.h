#pragma once
#include <Windows.h>
#include <string>

namespace Engine {
	class ALocalization
	{
	private:
		typedef signed int (__thiscall* _GetString)(void* pThis, int stringOffset);
		_GetString GetStringA;
	public:
		std::string GetString(int stringOffset);
		int* GetStringPointer(int stringOffset);
		ALocalization();
		ALocalization(uintptr_t base);
	};
}
