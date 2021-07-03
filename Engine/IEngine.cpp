#include "IEngine.h"

IEngine::IEngine()
{
	uintptr_t base = (uintptr_t)GetModuleHandle(NULL);

	Game::Player _Player(base);
	Player = _Player;

	Engine::ALocalization _ALocalization(base);
	ALocalization = _ALocalization;

	Engine::ScriptingAPI _ScriptingAPI(base, &Player);
	ScriptingAPI = _ScriptingAPI;

	Engine::AHUD _AHUD(base);
	AHUD = _AHUD;

	Engine::Particles _Particles(base);
	Particles = _Particles;

	CallTClassCTOR = (_CallTClassCTOR)0x005722a0;
}

void* IEngine::CreateTClass(int tClassInfoAddr)
{
	std::string tClassName = ReadHelper::ReadString(*(int*)(tClassInfoAddr));
	int* tClassAddr = CallTClassCTOR(*(void**)0x007894a4, tClassInfoAddr);
	char buf[512];

	sprintf_s(buf, "[TOSHI] %s was created at address %p\n", tClassName.c_str(), tClassAddr);
	printf(buf);

	return tClassAddr;
}