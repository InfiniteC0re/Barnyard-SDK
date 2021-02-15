#include "AHUD.h"

Engine::AHUD::AHUD(uintptr_t base)
{
	_base = base;
	GetGameResource = (_GetGameResource)(_base + 0x2BF890);
	OpenMainMenuA = (_OpenMainMenu)(_base + 0x9070);
}

void Engine::AHUD::OpenMainMenu()
{
	OpenMainMenuA((void*)(*(int*)((*(int*)(_base + 0x00381788)) + 0x2C)));
}
