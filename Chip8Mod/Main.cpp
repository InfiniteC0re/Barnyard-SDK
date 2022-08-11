#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <fstream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <chrono>

#include "detours.h"
#include "include/plugin.h"
#include "include/ToshiUtils.h"
#include "include/TPString8.h"
#include "include/ACameraManager.h"
#include "include/APlayerManager.h"
#include "include/AUnitManager.h"
#include "include/Math.h"

#include "include/Steer/AAlienSteer.h"
#include "include/InteractionActivities/ACameraActivity.h"

#include "emulator/devices/Chip8.h"

#include <d3d8.h>
#include <d3d8types.h>
#include <unordered_map>

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

#ifdef _DEBUG
#define DEBUG_PRINT(format, ...) printf("[Chip-8] "); printf(format, __VA_ARGS__);
#else
#define DEBUG_PRINT(format)
#endif

struct TextureInformation
{
	int m_flag;
	char* m_texName;
	int m_texSize;
	char* m_texData;
};

struct TTLTexturesData
{
	int count;
	TextureInformation* pTexInfos;
};

LPDIRECT3DTEXTURE8 g_ChipTexture;

Chip8 g_Chip;
bool g_IsRunning = true;
bool g_IsControlled = false;
int* g_CurrentWorldId = (int*)0x00772800;

// camera targets (for the screen lock mode)
static Vector4 s_CamTargetPos{ 31.00f, -10.20f, 5.65f, 1.0f };
static Vector4 s_CamTargetDir{ 33.50f, -9.70f, 7.65f, 1.0f };

thisCallHook(GetDDSTextureFromTTL, 0x00615d20, int*, bool, TTLTexturesData* ttlSymb)
{
	int* pTexturesInfo = ToshiUtils::TAlloc(ttlSymb->count * 0x10 + 4);
	TextureInformation* pVector = (TextureInformation*)(pTexturesInfo + 1);
	*pTexturesInfo = ttlSymb->count;

	ToshiUtils::MakeVector(pVector, sizeof(TextureInformation), ttlSymb->count, 0x00615ca0, 0x00615c90);
	*(TextureInformation**)(_this + 5) = pVector;
	*(TextureInformation**)(_this + 6) = pVector;
	*(int*)(_this + 7) = ttlSymb->count;

	for (int i = 0; i < ttlSymb->count; i++)
	{
		TextureInformation* texInfo = &ttlSymb->pTexInfos[i];
		ToshiUtils::CopyString(&pVector[i].m_texName, texInfo->m_texName);

		if (texInfo->m_flag == 1)
		{
			DDSTextureData* texData = (DDSTextureData*)ToshiUtils::TAlloc(sizeof(DDSTextureData));
			texData->m_unk1 = nullptr;
			texData->m_unk2 = nullptr;
			texData->m_texture = 0;

			texData->m_pTextureData = (char*)ToshiUtils::TAlloc(texInfo->m_texSize);

			if (texData->m_pTextureData != 0)
			{
				memcpy(texData->m_pTextureData, texInfo->m_texData, texInfo->m_texSize);
				texData->m_textureDataSize = texInfo->m_texSize;

				ToshiUtils::SaveDDSTexture(texData);

				if (strstr(texInfo->m_texName, "chip8_dummy"))
				{
					// replace the dummy texture with display texture
					texData->m_texture = g_ChipTexture;
				}

				*(DDSTextureData**)(&pVector[i].m_flag) = texData;
			}
		}
		else
		{
			*(DDSTextureData**)(&pVector[i].m_flag) = nullptr;
		}
	}

	ToshiUtils::CloseTRB((int*)(_this + 8));
	return true;
}

class Chip8Mod : public IPlugin
{
	bool Initialize();
	bool Unload();

	const char* GetName() const
	{
		return "Chip-8";
	}

	const char* GetVersion() const
	{
		return "1.0.0";
	}
};

IPlugin* IPlugin::Instance = new Chip8Mod();

DWORD APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved)
{
	return reason == DLL_PROCESS_ATTACH;
}

thisCallHook(WorldMaterial_SetupTexture, 0x005f6f70, void*, void)
{
	char* TRenderD3DInterface = *(char**)(0x007ce6c8);
	IDirect3DDevice8* pDevice = *(IDirect3DDevice8**)(TRenderD3DInterface + 0x678);

	realWorldMaterial_SetupTexture(_this, _edx);
	
	pDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_POINT);
	pDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_POINT);
	pDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTEXF_NONE);
}

enum class EventType
{
	GONE_DOWN,
	GONE_UP,
};

enum class ToshiKey
{
	KEY_1 = 0x20006, KEY_2 = 0x20007, KEY_3 = 0x20008, KEY_4 = 0x20009,
	KEY_Q = 0x20014, KEY_W = 0x20015, KEY_E = 0x20016, KEY_R = 0x20017,
	KEY_A = 0x20022, KEY_S = 0x20023, KEY_D = 0x20024, KEY_F = 0x20025, KEY_G = 0x20026, KEY_H = 0x20027,
	KEY_Z = 0x20030, KEY_X = 0x20031, KEY_C = 0x20032, KEY_V = 0x20033,
	KEY_U = 0x2001a, KEY_I = 0x2001b, KEY_O = 0x2001c, KEY_RETURN = 0x20020,
};

struct InputEvent
{
	ToshiKey m_Key;
	EventType m_EventType;
};

typedef void* (__thiscall* t_GetCameraPointer)(ACameraManager*);
typedef void(__thiscall* t_SetCameraPos)(void*, Vector4*);
typedef void(__thiscall* t_SetCameraLookAt)(void*, Vector4*);
typedef float(__stdcall* t_VectorsDistanceXZ)(Vector4*, Vector4*);
typedef bool(__stdcall* t_CallTrigger)(TPString8**, AUnit*, AUnit*);

static t_GetCameraPointer GetCameraPointer = (t_GetCameraPointer)0x0045b870;
static t_SetCameraLookAt SetCameraLookAt = (t_SetCameraLookAt)0x0045b2c0;
static t_SetCameraPos SetCameraPos = (t_SetCameraPos)0x0045b330;
static t_VectorsDistanceXZ VectorsDistanceXZ = (t_VectorsDistanceXZ)0x006cbaf0;
static t_CallTrigger CallTrigger = (t_CallTrigger)0x0041ab00;

void ResetChip()
{
	std::ifstream file;
	file.open("./rom.txt");
	std::string romName;
	file >> std::quoted(romName);
	file.close();

	g_Chip.Reset(false);
	g_Chip.LoadProgram(romName);
}

struct Coyote
{
	bool IsAttacked;
	ACharacterUnit* Unit;
	AInteriorSteer* Steer;
};

struct CoyotesBand
{
	float m_AvoidTime = 0;
	float m_AttackTime = 0;
	Vector3 m_MeetingPoint = { 0, 0, 0 };
	std::vector<Coyote> m_Coyotes;
};

CoyotesBand band;

void SpawnCoyote()
{
	// static
	static bool s_Spawned = false;

	// typedefs
	typedef void(__stdcall* t_HideChampionChart)(void);
	typedef void* (__thiscall* t_GetSkyBox)(void*, const char*);
	typedef void(__thiscall* t_ApplySkyBox)(void*, void*);
	typedef bool(__thiscall* t_IsNight)(void*);

	// hooks
	char* timeManager = *(char**)0x00783d3c;
	void* AGameTimeFXManager = *(void**)0x00783d08;
	static t_HideChampionChart HideChampionChart = (t_HideChampionChart)0x00603210;
	static t_GetSkyBox GetSkyBox = (t_GetSkyBox)0x005445e0;
	static t_ApplySkyBox ApplySkyBox = (t_ApplySkyBox)0x00545230;
	static t_IsNight IsNight = (t_IsNight)0x00546150;

	if (*g_CurrentWorldId != 0 || !g_AUnitManager || !g_APlayerManager) return;

	AUnitManager* pUnitManager = *g_AUnitManager;
	AUnitPlayer* player = (*g_APlayerManager)->GetCurPlayer(0);

	bool night = IsNight(timeManager);
	DEBUG_PRINT("IsNight: %d\n", night);

	if (!night)
	{
		static auto sleepTrigger = TPString8::Find("park2.triggers.sleep");
		CallTrigger(&sleepTrigger, player, player);
		return;
	}

	// stop the time
	*(bool*)(timeManager + 0x44) = true;

	// Set world type to 1
	//*(char*)0x007810e6 = 0x1;

	// Set something else to 1
	//*(char*)0x007810e0 = 0x0;

	ApplySkyBox(AGameTimeFXManager, GetSkyBox(AGameTimeFXManager, "Nightmare"));
	HideChampionChart();

	if (s_Spawned) return;

	static auto coyote = TPString8::Find("generic_coyote");
	static auto unitInfo = TObject::Find("AUnit", nullptr);
	static auto steerInfo = TObject::Find("ASteer", nullptr);
	static auto characterUnitInfo = TObject::Find("ACharacterUnit", unitInfo);
	static auto interiorSteerInfo = TObject::Find("AInteriorSteer", steerInfo);

	Vector4 pos{ 0, 0, 0, 1.0f };
	auto transform = Mat43::Make(&pos, (Vector4*)0x0077cfe4, 0, -1, -1, true);

	for (int i = 0; i < 7; i++)
	{
		auto coyoteUnit = (ACharacterUnit*)pUnitManager->CreateUnitByName(&coyote, &transform);
		auto coyoteSteer = (AInteriorSteer*)coyoteUnit->m_pSteer;

		assert(coyoteUnit->Info() == characterUnitInfo);
		assert(coyoteSteer->Info() == interiorSteerInfo);

		coyoteSteer->Command_SnapConnectNamedNetwork(TPString8::Find("park2"));
		coyoteUnit->SetDesiredSpeed(3.3f);

		band.m_Coyotes.emplace_back(false, coyoteUnit, coyoteSteer);
	}

	s_Spawned = true;
}

thisCallHook(ProcessInput, 0x0058c7a0, void*, bool, InputEvent* pEvent)
{
	static float originalFov = *(float*)(0x007822ac);

	if (pEvent->m_Key == ToshiKey::KEY_H && pEvent->m_EventType == EventType::GONE_DOWN)
	{
		SpawnCoyote();
		
		return true;
	}

	if (pEvent->m_Key == ToshiKey::KEY_O && pEvent->m_EventType == EventType::GONE_DOWN)
	{
		if (g_Chip.IsPaused()) return true;

		char* camera = (char*)GetCameraPointer(*g_ACameraManager);
		auto tClass = TObject::Find("ACameraActivity", nullptr);

		g_IsControlled = !g_IsControlled;

		if (g_IsControlled)
		{
			// update FOV
			*(float*)(camera + 0x44) = 0.85f;

			if (tClass)
			{
				ACameraActivityInfo* camActivity = (ACameraActivityInfo*)tClass->Create();
				camActivity->SetMode(TPString8::Find("nohelpercameras"));
				camActivity->Action();
				camActivity->Delete(true);

				SetCameraPos(camera, &s_CamTargetPos);
				SetCameraLookAt(camera, &s_CamTargetDir);
			}
		}
		else
		{
			// reset FOV
			*(float*)(camera + 0x44) = originalFov;

			ACameraActivityInfo* camActivity = (ACameraActivityInfo*)tClass->Create();
			camActivity->SetMode(TPString8::Find("simcamera"));
			camActivity->Action();
			camActivity->Delete(true);
		}

		return true;
	}

	if (g_IsControlled)
	{
		auto& keyboard = g_Chip.GetKeyboard();
		bool isPressed = pEvent->m_EventType == EventType::GONE_DOWN;

		if (pEvent->m_Key == ToshiKey::KEY_RETURN)
		{
			// reset chip8
			ResetChip();

			return true;
		}
		
		switch (pEvent->m_Key)
		{
		case ToshiKey::KEY_1:
			keyboard.SetKeyState(KeyboardKey::KEY_1, isPressed);
			break;
		case ToshiKey::KEY_2:
			keyboard.SetKeyState(KeyboardKey::KEY_2, isPressed);
			break;
		case ToshiKey::KEY_3:
			keyboard.SetKeyState(KeyboardKey::KEY_3, isPressed);
			break;
		case ToshiKey::KEY_4:
			keyboard.SetKeyState(KeyboardKey::KEY_C, isPressed);
			break;

		case ToshiKey::KEY_Q:
			keyboard.SetKeyState(KeyboardKey::KEY_4, isPressed);
			break;
		case ToshiKey::KEY_W:
			keyboard.SetKeyState(KeyboardKey::KEY_5, isPressed);
			break;
		case ToshiKey::KEY_E:
			keyboard.SetKeyState(KeyboardKey::KEY_6, isPressed);
			break;
		case ToshiKey::KEY_R:
			keyboard.SetKeyState(KeyboardKey::KEY_D, isPressed);
			break;

		case ToshiKey::KEY_A:
			keyboard.SetKeyState(KeyboardKey::KEY_7, isPressed);
			break;
		case ToshiKey::KEY_S:
			keyboard.SetKeyState(KeyboardKey::KEY_8, isPressed);
			break;
		case ToshiKey::KEY_D:
			keyboard.SetKeyState(KeyboardKey::KEY_9, isPressed);
			break;
		case ToshiKey::KEY_F:
			keyboard.SetKeyState(KeyboardKey::KEY_E, isPressed);
			break;

		case ToshiKey::KEY_Z:
			keyboard.SetKeyState(KeyboardKey::KEY_A, isPressed);
			break;
		case ToshiKey::KEY_X:
			keyboard.SetKeyState(KeyboardKey::KEY_0, isPressed);
			break;
		case ToshiKey::KEY_C:
			keyboard.SetKeyState(KeyboardKey::KEY_B, isPressed);
			break;
		case ToshiKey::KEY_V:
			keyboard.SetKeyState(KeyboardKey::KEY_F, isPressed);
			break;
		}

		return true;
	}
	else
	{
		return realProcessInput(_this, _edx, pEvent);
	}
}

thisCallHook(BeginScene, 0x006c6590, char*, bool)
{
	bool result = realBeginScene(_this, _edx);
	
	//IDirect3DDevice8* pDevice = *(IDirect3DDevice8**)(_this + 0x678);
	g_Chip.GetDisplay().Update(g_ChipTexture);

	return result;
}

thisCallHook(EndScene, 0x006c6fd0, char*, bool)
{
	return realEndScene(_this, _edx);
}

thisCallHook(GameTick, 0x006bbb00, char*, void)
{
	realGameTick(_this, _edx);

	float deltaTime = *(float*)(*(char**)(*(int*)(_this + 0xC) + 0x4) + 0x1C);

	// Game logic
	if (*g_APlayerManager != nullptr)
	{
		AUnitPlayer* player = (*g_APlayerManager)->GetCurPlayer(0);
		bool isChipPaused = g_Chip.IsPaused();

		if (*g_CurrentWorldId != 0 || player == nullptr)
		{
			g_Chip.SetPaused(true);
		}
		else
		{
			// the loaded world is EnvMain
			bool shouldPause;
			AAlienSteer* steer = (AAlienSteer*)player->m_pSteer;

			static TPString8* triggerName = TPString8::Find("coyote_spot");
			static TPString8* yukAnim = TPString8::Find("yuk");
			
			if (steer->Info() == (TObjectInfo*)0x00788b68)
			{
				band.m_AvoidTime = 5.0f;
			}

			for (auto& coyote : band.m_Coyotes)
			{
				float distance = player->GetDistanceSqToUnit(coyote.Unit);
				bool isAttacked = coyote.Unit->m_AnimatableObject->IsAnimation(&yukAnim);

				if (isAttacked) coyote.Steer->Command_StateStill();
				else coyote.Steer->Command_EndStateStill();

				if (coyote.IsAttacked != isAttacked)
				{
					if (isAttacked)
					{
						band.m_AvoidTime = 4.0f;
					}

					coyote.IsAttacked = isAttacked;
				}

				if (band.m_AvoidTime <= 0)
				{
					if (distance <= 3.0f && band.m_AttackTime <= 0.0f)
					{
						coyote.Steer->FaceUnit(player);
						CallTrigger(&triggerName, player, player);

						band.m_AttackTime = 1.8f;
						band.m_AvoidTime = 2.0f;
					}
					else
					{
						coyote.Steer->Command_SeekUnitPointOver(player);
					}
				}
				else
				{
					coyote.Steer->Command_AvoidUnit(player, 20.0f, true);
				}
			}

			band.m_AvoidTime -= deltaTime;
			band.m_AttackTime -= deltaTime;

			// 0x007892f4 - APlayerSteerObjectInfo
			// 0x00788ac4 - AAlienSteerObjectInfo
			auto info = steer->Info();

			if (info == (TObjectInfo*)0x007892f4 || info == (TObjectInfo*)0x00788ac4)
			{
				auto kinematics = steer->m_pKinematics3D;

				float distance = VectorsDistanceXZ(&kinematics->m_vPosition, &s_CamTargetPos);
				shouldPause = distance >= 50;
			}
			else
			{
				shouldPause = true;
			}

			if (isChipPaused != shouldPause)
			{
				DEBUG_PRINT("Switched state: %d\n", shouldPause);
				auto& display = g_Chip.GetDisplay();

				if (shouldPause)
				{
					DEBUG_PRINT("Saved screen state\n");
					display.Backup();
					display.Clear();
				}
				else
				{
					DEBUG_PRINT("Restored screen state\n");
					display.Restore();
				}

				g_Chip.SetPaused(shouldPause);
			}
		}
	}
}

DWORD WINAPI MainThread(HMODULE hModule)
{
	double lastTimerTick = 0;
	double lastCPUTick = 0;

	double nextSecond = 1.0;
	int times = 0;

	auto start = std::chrono::system_clock::now();

	while (g_IsRunning)
	{
		if (!g_Chip.IsPaused() && !g_Chip.HasExited())
		{
			// Chip-8
			auto time = std::chrono::system_clock::now();
			std::chrono::duration<double> currTime = time - start;

			double cpuTickDiff = currTime.count() - lastCPUTick;
			double timerTickDiff = currTime.count() - lastTimerTick;

			// updating CPU 
			if (cpuTickDiff >= CPU_HZ_SECONDS)
			{
				lastCPUTick = currTime.count();
				times++;

				g_Chip.GetCPU().TickCPU();
			}

			// updating timers
			if (timerTickDiff >= CPU_TIMER_HZ_SECONDS)
			{
				lastTimerTick = currTime.count();
				g_Chip.GetCPU().TickTimers();
			}
		}
		else
		{
			Sleep(50);
			continue;
		}
	}

	return 0;
}

bool Chip8Mod::Initialize()
{
	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	// attach here
	Attach(BeginScene);
	Attach(EndScene);
	Attach(GetDDSTextureFromTTL);
	Attach(WorldMaterial_SetupTexture);
	Attach(ProcessInput);
	Attach(GameTick);

	DetourTransactionCommit();
	
	char* TRenderD3DInterface = *(char**)(0x007ce6c8);
	IDirect3DDevice8* pDevice = *(IDirect3DDevice8**)(TRenderD3DInterface + 0x678);

	pDevice->CreateTexture(64, 32, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_ChipTexture);
	DEBUG_PRINT("Texture Pointer: %p\n", g_ChipTexture);

	ResetChip();
	CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0));

	return true;
}

bool Chip8Mod::Unload()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	
	Dettach(BeginScene);
	Dettach(EndScene);
	Dettach(GetDDSTextureFromTTL);
	Dettach(WorldMaterial_SetupTexture);
	Dettach(ProcessInput);
	Dettach(GameTick);

	DetourTransactionCommit();

	delete Chip8Mod::Instance;
	FreeLibrary(this->hModule);
	DEBUG_PRINT("Unloaded Chip8 mod...");

	return true;
}

extern "C" __declspec(dllexport) IPlugin * GetPluginInstance()
{
	return Chip8Mod::Instance;
}