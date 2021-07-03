#include "AHUD.h"
#include <iostream>

Engine::AHUD::AHUD() { }

Engine::AHUD::AHUD(uintptr_t base)
{
	_base = base;
	GetGameResource = (_GetGameResource)(_base + 0x2BF890);
	OpenMainMenuA = (_OpenMainMenu)(_base + 0x9070);
	GetFont = (_GetFont)(_base + 0x2C44D0);
	SetButtonTextA = (_SetButtonText)(_base + 0x240490);
}

void Engine::AHUD::SetButtonText(int* pFont, int* pText, int buttonID)
{
	SetButtonTextA((void*)((*(int*)((*(int*)(_base + 0x00381788) + 0x2c))) + (0x7D8 + 0x204 * buttonID)), pFont, 800, pText, 0);
}

void Engine::AHUD::OpenMainMenu()
{
	OpenMainMenuA(*(void**)(((*(int*)(_base + 0x00381788)) + 0x2C)));
}
