#pragma once
#include <Windows.h>
#include <string>

namespace Engine {
	class AHUD
	{
	private:
		uintptr_t _base;
		typedef signed int(__stdcall* _GetGameResource)(const char* resourceName);
		typedef signed int(__thiscall* _OpenMainMenu)(void* pThis);
		_OpenMainMenu OpenMainMenuA;
	public:
		_GetGameResource GetGameResource;
		void OpenMainMenu();
		AHUD(uintptr_t base);
	};
}
