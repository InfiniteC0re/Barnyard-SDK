#pragma once
#include <Windows.h>
#include <string>

#include "Game\Player.h"

namespace Engine {
	class ScriptingAPI
	{
	private:
		Game::Player* pPlayer;
		typedef void(__cdecl* _ShowChampionChart)(void);
		typedef void(__cdecl* _HideChampionChart)(void);
		typedef void(__stdcall* _SaveCurrentGameState)(int flag1, int flag2, int flag3);
		typedef int*(__thiscall* _FindUnitByTypeName)(void* pThis, int flag1);
		typedef int*(__thiscall* _GetCurPlayer)(void *pThis, int flag1);
		typedef int* (__thiscall* _GetSkyboxAddress)(void* pThis, const char* name);
		typedef int* (__thiscall* _ApplySkybox)(void* pThis, int*);
		typedef void(__thiscall* _OpenClosestGate)(void* pThis);
		typedef void(__stdcall* _GameMessage)(int titleLocaleID, int contentLocaleID);
		typedef void(__stdcall* _SpawnGBucks)(int count, Vector4* coordinates);
		_OpenClosestGate OpenClosestGateA;
		_GetSkyboxAddress GetSkyboxAddressA;
		_ApplySkybox ApplySkyboxA;
		_ShowChampionChart ShowChampionChartA;
		_HideChampionChart HideChampionChartA;
		bool isEnvMain();
	public:
		_GameMessage GameMessage;
		_SpawnGBucks SpawnGBucks;
		_SaveCurrentGameState SaveCurrentGameState;
		_FindUnitByTypeName FindUnitByTypeName;
		_GetCurPlayer GetCurPlayer;
		void ShowChampionChart();
		void HideChampionChart();
		void OpenClosestGate();
		int* GetSkyboxAddress(const char* skyboxName);
		void ApplySkybox(int* skyboxAddress);
		ScriptingAPI();
		ScriptingAPI(uintptr_t base, Game::Player* Player);
	};
}
