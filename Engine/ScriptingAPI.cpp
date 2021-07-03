#include "ScriptingAPI.h"

bool Engine::ScriptingAPI::isEnvMain()
{
	pPlayer->UpdateLocation();

	return pPlayer->getWorldID() == 0;
}

int* Engine::ScriptingAPI::GetSkyboxAddress(const char* skyboxName)
{
	return GetSkyboxAddressA(*(void**)(0x783D08), skyboxName);
}

void Engine::ScriptingAPI::ApplySkybox(int* skyboxAddress)
{
	ApplySkyboxA(*(void**)(0x783D08), skyboxAddress);
}

void Engine::ScriptingAPI::OpenClosestGate()
{
	OpenClosestGateA(*(void**)(0x0078de44));
}

void Engine::ScriptingAPI::ShowChampionChart()
{
	if (isEnvMain()) ShowChampionChartA();
}

void Engine::ScriptingAPI::HideChampionChart()
{
	if (isEnvMain()) HideChampionChartA();
}

Engine::ScriptingAPI::ScriptingAPI() {}

Engine::ScriptingAPI::ScriptingAPI(uintptr_t base, Game::Player* Player)
{
	pPlayer = Player;
	ShowChampionChartA = (_ShowChampionChart)(base + 0x2031A0);
	HideChampionChartA = (_HideChampionChart)(base + 0x203210);
	OpenClosestGateA = (_OpenClosestGate)0x005dee90;
	SaveCurrentGameState = (_SaveCurrentGameState)(base + 0x2a230);
	FindUnitByTypeName = (_FindUnitByTypeName)(base + 0x10250);
	GetCurPlayer = (_GetCurPlayer)(base + 0x220420);
	GetSkyboxAddressA = (_GetSkyboxAddress)(base + 0x1445e0);
	ApplySkyboxA = (_ApplySkybox)(base + 0x145230);
	GameMessage = (_GameMessage)0x00602e90;
	SpawnGBucks = (_SpawnGBucks)0x0066f2a0;
}