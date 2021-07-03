#pragma once
#include <Windows.h>
#include ".\ALocalization.h"
#include ".\AHUD.h"
#include ".\Particles.h"
#include ".\ScriptingAPI.h"
#include ".\Game\Player.h"

class IEngine
{
private:
	typedef int* (__thiscall* _CallTClassCTOR)(void* pSteerPool, int tClassInfo);
	_CallTClassCTOR CallTClassCTOR;
public:
	Engine::ALocalization ALocalization;
	Engine::ScriptingAPI ScriptingAPI;
	Game::Player Player;
	Engine::AHUD AHUD;
	Engine::Particles Particles;
	void* CreateTClass(int TClassInfoAddr);
	IEngine();
};

