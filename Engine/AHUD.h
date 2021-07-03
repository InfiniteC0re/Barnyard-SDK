#pragma once
#include <Windows.h>
#include <string>

namespace Engine {
	class AHUD
	{
	private:
		uintptr_t _base;
		typedef int* (__stdcall* _GetFont)(const char* fontPath);
		typedef signed int(__stdcall* _GetGameResource)(const char* resourceName);
		typedef signed int(__thiscall* _OpenMainMenu)(void* pThis);
		typedef void(__thiscall* _SetButtonText)(void* pThis, int* pFont, int width, int* pText, int flag);
		_OpenMainMenu OpenMainMenuA;
		_SetButtonText SetButtonTextA;
	public:
		_GetFont GetFont;
		_GetGameResource GetGameResource;
		void OpenMainMenu();
		void SetButtonText(int* pFont, int* pText, int buttonID);
		AHUD();
		AHUD(uintptr_t base);
	};
}
