#include "ALocalization.h"
#include "../Utils/ReadHelper.cpp"

Engine::ALocalization::ALocalization() { }

Engine::ALocalization::ALocalization(uintptr_t base)
{
	GetStringA = (_GetString)(base + 0x2BE7B0);
}

std::string Engine::ALocalization::GetString(int stringOffset)
{
	return ReadHelper::ReadString(GetStringA(*(void**)0x7ce6bc, stringOffset));
}

int* Engine::ALocalization::GetStringPointer(int stringOffset)
{
	return (int*)GetStringA(*(void**)0x7ce6bc, stringOffset);
}