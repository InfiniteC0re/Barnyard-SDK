#include <iostream>
#include <Windows.h>
#include <fstream>
#include <stdio.h>
#include <detours.h>
#include <atlstr.h>
#include ".\Engine\IEngine.h"
#include ".\Structs\AAlienSteer.h"
#include ".\Structs\AUnit.h"
#include ".\Structs\Render.h"


//#pragma comment(lib, "D:\\Programs\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.28.29910\\lib\\x86\\libcmtd.lib")
//#pragma comment(lib, "D:\\Programs\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.28.29910\\lib\\x86\\legacy_stdio_definitions.lib")
#pragma comment(lib, "detours.lib")

#define thisCallHook(fnName, addr, thisType, retType, ...) \
    typedef retType (__fastcall fnName)(thisType _this, DWORD _edx, __VA_ARGS__); \
    fnName* real##fnName = (fnName*)addr; \
    retType __fastcall _##fnName(thisType _this, DWORD _edx, __VA_ARGS__)

#define Attach(fnName) DetourAttach(&(PVOID&)real##fnName, _##fnName);
#define Dettach(fnName) DetourDetach(&(PVOID&)real##fnName, _##fnName);

IEngine *pEngine;

#pragma region AGameTimeManager
typedef unsigned char   undefined;
typedef unsigned int    undefined4;

struct GameTimeInfo {
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
	undefined field_0x4;
	undefined field_0x5;
	undefined field_0x6;
	undefined field_0x7;
	undefined field_0x8;
	undefined field_0x9;
	undefined field_0xa;
	undefined field_0xb;
	undefined field_0xc;
	undefined field_0xd;
	undefined field_0xe;
	undefined field_0xf;
	undefined field_0x10;
	undefined field_0x11;
	undefined field_0x12;
	undefined field_0x13;
	struct AGameTimeManager* pGameTimeManager;
	undefined field_0x18;
	undefined field_0x19;
	undefined field_0x1a;
	undefined field_0x1b;
	float deltaTime;
	float field_0x20;
	float field_0x24;
	int field_0x28;
	undefined* field_0x2c;
	char field_0x30;
	undefined field_0x31;
	undefined field_0x32;
	undefined field_0x33;
	float field_0x34;
	undefined field_0x38;
	undefined field_0x39;
	undefined field_0x3a;
	undefined field_0x3b;
	undefined field_0x3c;
	char field_0x3d;
	undefined field_0x3e;
	undefined field_0x3f;
	float field_0x40;
};

struct AGameTimeManager {
	undefined** functions;
	undefined4 field_0x4;
	undefined4 field_0x8;
	void* field_0xc;
	void* field_0x10;
	undefined4 field_0x14;
	struct AGameTimeManager* possibleTimeManager;
	undefined4 field_0x1c;
	undefined4 field_0x20;
	undefined4 field_0x24;
	undefined4 field_0x28;
	undefined4 field_0x2c;
	int field_0x30;
	undefined4 field_0x34;
	float environmentTime;
	void* field_0x3c;
	float deltaSec;
	byte field_0x44;
	undefined field_0x45;
	undefined field_0x46;
	undefined field_0x47;
	float dayTime;
	byte field_0x4c;
	byte field_0x4d;
	undefined field_0x4e;
	undefined field_0x4f;
};

#pragma endregion

#pragma region Speedhack

float multiplier = 1;

thisCallHook(GameTick, 0x006bcc80, GameTimeInfo*, void, AGameTimeManager* pGameTimeManager)
{
	char s[256];

	if (*(int**)0x007822E0 == 0) // check if ACameraManager does exist
		_this->deltaTime = 10; // skip loading screens
	else
		_this->deltaTime *= multiplier; // apply speedhack

	sprintf_s(s, "[Engine] Game Tick (Delta Time: %fs; Multiplier: %f)\n", _this->deltaTime, multiplier);
	//printf(s);

	realGameTick(_this, _edx, pGameTimeManager);
}

#pragma endregion

#pragma region StoryManager

thisCallHook(GetAbsoluteCurrentMission, 0x005cbf90, void*, int, int act, int mission)
{
	int absolute = realGetAbsoluteCurrentMission(_this, _edx, act, mission);

	char s[256];
	sprintf_s(s, "Absolute: %d; Act: %d; Mission: %d\n", absolute, act, mission);
	//printf(s);

	return absolute;
}

thisCallHook(GetMissionActAndID, 0x005ccfb0, void*, void, char* name, int* act, int* mission)
{
	realGetMissionActAndID(_this, _edx, name, act, mission);

	char s[256];
	sprintf_s(s, "Name: %s; Act: %d; Mission: %d\n", name, *(int*)act, *(int*)mission);
	//printf(s);
}

#pragma endregion

#pragma region Units

AUnit* Units[256];
int lastUnit = 0;

thisCallHook(GetUnitFromSteer, 0x00555e00, AAlienSteer*, AUnit*)
{
	AUnit* pUnit = realGetUnitFromSteer(_this, _edx);
	Vector4 coordinates = pUnit->coordinates;

	bool saved = false;

	for (int i = 0; i < 256; i++)
	{
		if (Units[i] == pUnit)
		{
			saved = true;
			break;
		}
	}

	if (!saved) 
		Units[lastUnit++] = pUnit;

	return pUnit;
}

#pragma endregion

#pragma region CustomHUD

RECT rect1;
RECT rect2;
HFONT hFont;
//LPD3DXFONT g_pD3DXFont;
TRenderD3DInterface* pRender;

const char* skyboxes[] = { "dawn", "Golf7", "Golf7", "dusk", "Golf7", "dawnheatwave", "morningheatwave", "afternoonheatwave", "duskheatwave", "nightheatwave", "dawnlightrain", "MorningLightRain", "afternoonlightrain", "dusklightrain", "nightlightrain", "dawnHeavyrain", "MorningHeavyRain", "afternoonHeavyrain", "DuskHeavyRain", "nightHeavyrain", "COWTIP", "CARLIGHT", "Golf1", "Golf2", "Golf3", "Golf4", "Golf5", "Golf6", "Golf7", "Golf8", "Golf9", "Whack", "gophershop", "Woods", "Honey", "PeckRange", "Gatecrasher", "BarnNight", "Cocktail", "Pool", "Tapper", "JunkDay", "JunkNight", "FinalMission", "BeadyInt", "ChickenCoopInt", "FarmHouseInt", "Golf10", "Nightmare", "Golf11", "Golf12", "Golf13", "Golf14", "Golf15", "Golf16", "Golf17", "Golf18", "CowTipDawn", "Darts", "CowTipMoon", "BarnDay", "BeadyDay" };
int skyboxIndex = 0;

void InitializeFont()
{
	/*D3DVIEWPORT8 vp;
	pRender->pDevice->GetViewport(&vp);

	hFont = CreateFontW(36, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, PROOF_QUALITY, 0, L"Arial");
	rect1 = { 0, 0, (long)vp.Width - 8, (long)vp.Height - 8 };
	rect2 = { 0, 0, (long)vp.Width - 8, (long)vp.Height - 40 };
	D3DXCreateFont(pRender->pDevice, hFont, &g_pD3DXFont);*/
}

thisCallHook(EndScene, 0x006c6fd0, TRenderD3DInterface*, int)
{
	if (pRender == (TRenderD3DInterface*)0x0)
	{
		pRender = _this;
		InitializeFont();
	}

	if (pEngine != (IEngine*)0x0 && _this->isSceneStarted == true)
	{	
		pEngine->Player.UpdatePosition();
		Vector4 *pos = pEngine->Player.getPosition();

		if (pos != (Vector4*)0x0)
		{
			/*wchar_t msg[256];
			swprintf_s(msg, L"Pos (%.2f, %.2f, %.2f)", pos->x, pos->y, pos->z);
			g_pD3DXFont->DrawText(msg, -1, &rect1, DT_RIGHT | DT_BOTTOM, D3DCOLOR_RGBA(255, 255, 255, 255));

			wchar_t skyboxName[32];
			std::mbstowcs(skyboxName, skyboxes[skyboxIndex], 32);
			swprintf_s(msg, L"Selected Skybox: %s", skyboxName);
			g_pD3DXFont->DrawText(msg, -1, &rect2, DT_RIGHT | DT_BOTTOM, D3DCOLOR_RGBA(255, 255, 255, 255));*/
		}
	}

	return realEndScene(_this, _edx);
}

#pragma endregion

DWORD WINAPI MainThread(HMODULE hModule)
{
	typedef int* (__thiscall* _ActivateCheat)(void* pThis, int id);
	_ActivateCheat ActivateCheat = (_ActivateCheat)(0x00422d10);

	IEngine Engine;
	pEngine = &Engine;

	DetourRestoreAfterWith();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	Attach(GameTick);
	Attach(GetAbsoluteCurrentMission);
	Attach(GetMissionActAndID);
	Attach(GetUnitFromSteer);
	Attach(EndScene);
	DetourTransactionCommit();

	printf("[Detour] Done!\n");

	while (!GetAsyncKeyState(VK_END))
	{
		if (GetAsyncKeyState(VK_OEM_PLUS) & 1)
		{
			multiplier += 0.1f;
		}
		else if (GetAsyncKeyState(VK_OEM_MINUS) & 1)
		{
			multiplier -= 0.1f;
		}
		else if (GetAsyncKeyState(VK_F1) & 1)
		{
			int* skybox = Engine.ScriptingAPI.GetSkyboxAddress(skyboxes[skyboxIndex]);
			Engine.ScriptingAPI.ApplySkybox(skybox);
		}
		else if (GetAsyncKeyState(VK_F2) & 1)
		{
			if (skyboxIndex > 0) skyboxIndex--;
			int* skybox = Engine.ScriptingAPI.GetSkyboxAddress(skyboxes[skyboxIndex]);
			Engine.ScriptingAPI.ApplySkybox(skybox);
			std::cout << skyboxIndex << std::endl;
		}
		else if (GetAsyncKeyState(VK_F3) & 1)
		{
			if (skyboxIndex + 1 < 62) skyboxIndex++;
			int* skybox = Engine.ScriptingAPI.GetSkyboxAddress(skyboxes[skyboxIndex]);
			Engine.ScriptingAPI.ApplySkybox(skybox);
			std::cout << skyboxIndex << std::endl;
		}
		else if (GetAsyncKeyState(VK_F5) & 1)
		{
			Engine.ScriptingAPI.HideChampionChart();
		}

		Sleep(50);
	}

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	Dettach(GameTick);
	Dettach(GetAbsoluteCurrentMission);
	Dettach(GetMissionActAndID);
	Dettach(GetUnitFromSteer);
	Dettach(EndScene);
	DetourTransactionCommit();

	printf("Exiting...\n");

	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

DWORD APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		AllocConsole();
		FILE* fDummy;
		freopen_s(&fDummy, "CONOUT$", "w", stdout);
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0));
	}

	return TRUE;
}