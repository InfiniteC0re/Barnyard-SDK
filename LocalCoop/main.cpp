#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <iostream>
#include <Windows.h>
#include "detours.h"

#include "include/plugin.h"
#include "include/ToshiUtils.h"
#include "include/AUnitManager.h"
#include "include/APlayerManager.h"
#include "include/Steer/AAlienSteer.h"
#include "include/ACameraManager.h"
#include "include/AInteractionComponent.h"
#include "include/APlayerControlComponent.h"

#define thisCallHook(fnName, addr, thisType, retType, ...) \
    typedef retType (__fastcall fnName)(thisType _this, DWORD _edx, __VA_ARGS__); \
    fnName* real##fnName = (fnName*)addr; \
    retType __fastcall _##fnName(thisType _this, DWORD _edx, __VA_ARGS__)

#define stdCallHook(fnName, addr, retType, ...) \
    typedef retType (__stdcall fnName)(__VA_ARGS__); \
    fnName* real##fnName = (fnName*)addr; \
    retType __stdcall _##fnName(__VA_ARGS__)
	
#define Attach(fnName) DetourAttach(&(PVOID&)real##fnName, _##fnName);
#define Dettach(fnName) DetourDetach(&(PVOID&)real##fnName, _##fnName);

AUnitPlayer* pLastControlledPlayer = nullptr;
AUnitPlayer* pFakePlayer = nullptr;
AUnitPlayer* pSecondPlayer = nullptr;

thisCallHook(AShakeTreeMicroGameDestructor, 0x0052db70, void*, void*, char flag)
{
	std::cout << "TreeMicroGame Destructor" << std::endl;
	pFakePlayer = nullptr;
	return realAShakeTreeMicroGameDestructor(_this, _edx, flag);
}

thisCallHook(AUnitPlayerDestructor, 0x00414990, AUnitPlayer*, void*, char flag)
{
	std::cout << "UnitPlayer Destructor" << std::endl;

	if (pSecondPlayer == _this)
		pSecondPlayer = nullptr;

	if (pFakePlayer == _this)
		pFakePlayer = nullptr;

	if (pLastControlledPlayer == _this)
		pLastControlledPlayer = nullptr;

	return realAUnitPlayerDestructor(_this, _edx, flag);
}

thisCallHook(GetCurPlayer, 0x0061faf0, APlayerManager*, AUnitPlayer*, int index)
{
	if (pFakePlayer) return pFakePlayer;
	return realGetCurPlayer(_this, _edx, index);
}

class CoopMod : public IPlugin
{
	bool Initialize();
	bool Unload();

	const char* GetName() const
	{
		return "Coop";
	}

	const char* GetVersion() const
	{
		return "0.0.1-alpha";
	}
};

IPlugin* CoopMod::Instance = new CoopMod;

DWORD WINAPI MainThread(HMODULE hModule)
{
	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	Attach(AShakeTreeMicroGameDestructor);
	Attach(AUnitPlayerDestructor);
	Attach(GetCurPlayer);
	DetourTransactionCommit();

	double controlledRot = 0;
	TObjectInfo* AInteractionComponentInfo = TObject::Find("AInteractionComponent", 0x0);
	TObjectInfo* ASimplePlayerControlComponentInfo = TObject::Find("APlayerControlComponent", 0x0);

	while (CoopMod::Instance)
	{
		if (GetAsyncKeyState(VK_F1) & 1)
		{
			AUnitManager* pUnitManager = *g_AUnitManager;
			AUnitPlayer* pPlayer = (*g_APlayerManager)->GetCurPlayer(0);

			if (pPlayer != nullptr)
			{
				TPString8* pPlayerStr = TPString8::Find("player");
				auto playerSteer = (AAlienSteer*)pPlayer->m_pSteer;
				auto steerInfo = playerSteer->Info();

				if (steerInfo == (TObjectInfo*)0x007892f4 || steerInfo == (TObjectInfo*)0x00788ac4)
				{
					Vector4 rot{ 1, 0, 0, 1 };
					auto transform = Mat43::Make(&playerSteer->m_pKinematics3D->m_vPosition, &rot, 0, 0, -1, true);

					AUnit* pUnit = pUnitManager->CreateUnitByName(&pPlayerStr, &transform);

					if (strcmp(pUnit->Info()->className, "AUnitPlayer") == 0)
					{
						pSecondPlayer = TCAST(AUnitPlayer*, pUnit);
					}
				}
			}
		}

		if (pSecondPlayer != nullptr)
		{
			AAlienSteer* steer = (AAlienSteer*)pSecondPlayer->m_pSteer;
			Vector4 speedVec{ 0, 0, 0, 0 };

			if (GetAsyncKeyState(VK_NUMPAD8) < 0)
			{
				speedVec.x = 4;
			}
			else if (GetAsyncKeyState(VK_NUMPAD4) || GetAsyncKeyState(VK_NUMPAD6))
			{
				speedVec.x = 4;
				controlledRot += GetAsyncKeyState(VK_NUMPAD4) ? 0.07 : -0.07;

				Vector4 rotVec{ cos(controlledRot), 0, sin(controlledRot), 1 };
				steer->m_pKinematics3D->m_qLastRotation = rotVec;
			}

			if (GetAsyncKeyState(VK_NUMPAD5) & 1)
			{
				AAlienSteer* steer = (AAlienSteer*)pSecondPlayer->m_pSteer;
				steer->Jump();
			}

			if (GetAsyncKeyState(VK_NUMPAD9) & 1)
			{
				AInteractionComponent* pInteraction = TCAST(AInteractionComponent*, pSecondPlayer->FindAttachedToshiClass(AInteractionComponentInfo));
				APlayerControlComponent* pControl = TCAST(APlayerControlComponent*, pSecondPlayer->FindAttachedToshiClass(ASimplePlayerControlComponentInfo));

				if (pControl)
				{
					AInteractionSequence* pSq = pControl->CallInteractionSequence();

					if (pSq != nullptr) pFakePlayer = pSecondPlayer;
				}
			}

			if (GetAsyncKeyState(VK_NUMPAD7) & 1)
			{
				if (pSecondPlayer)
				{
					AUnitPlayer* pControlled = (*g_ACameraManager)->m_pCameraHelper->m_pProxy->pPlayer;
					if (pLastControlledPlayer == nullptr) pLastControlledPlayer = pSecondPlayer;
					(*g_ACameraManager)->m_pCameraHelper->m_pProxy->pPlayer = pLastControlledPlayer;

					pLastControlledPlayer = pControlled;
				}
			}

			steer->SetMovementSpeed(&speedVec);
		}

		Sleep(25);
	}

	return 0;
}

DWORD APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved)
{
	return reason == DLL_PROCESS_ATTACH;
}

bool CoopMod::Initialize()
{
	CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0));
	return true;
}

bool CoopMod::Unload()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	Dettach(AShakeTreeMicroGameDestructor);
	Dettach(AUnitPlayerDestructor);
	Dettach(GetCurPlayer);
	DetourTransactionCommit();

	delete CoopMod::Instance;
	CoopMod::Instance = 0;

	FreeLibrary(hModule);
	std::cout << "[Coop Mod] Unloaded Coop Mod..." << std::endl;

	return true;
}

extern "C" __declspec(dllexport) IPlugin * GetPluginInstance()
{
	return CoopMod::Instance;
}