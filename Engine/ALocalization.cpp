#include "ALocalization.h"
#include "../Utils/ReadHelper.cpp"

Engine::ALocalization::ALocalization(uintptr_t base)
{
	_base = base;
	GetStringA = (_GetString)(_base + 0x2BE7B0);
}

std::string Engine::ALocalization::GetString(int stringOffset)
{
	return ReadHelper::ReadString(GetStringA(*(void**)0x7ce6bc, stringOffset));
}