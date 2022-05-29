#include <iostream>
#include <windows.h>
#include <filesystem>
#include "include/plugin.h"
#include "detours.h"

namespace fs = std::filesystem;

#define thisCallHook(fnName, addr, thisType, retType, ...) \
    typedef retType (__fastcall fnName)(thisType _this, DWORD _edx, __VA_ARGS__); \
    fnName* real##fnName = (fnName*)addr; \
    retType __fastcall _##fnName(thisType _this, DWORD _edx, __VA_ARGS__)

#define Attach(fnName) DetourAttach(&(PVOID&)real##fnName, _##fnName);
#define Dettach(fnName) DetourDetach(&(PVOID&)real##fnName, _##fnName);

typedef IPlugin* (__stdcall* GetPluginInstance)();
std::vector<IPlugin*> plugins;
HMODULE hModuleCore;

// 0x006c6da0 - ~TRenderD3DInterface()
thisCallHook(TRenderD3DInterfaceDestructor, 0x006c6da0, void*, void*, char flag)
{
	std::cout << "[Modloader] Unloading " << plugins.size() << " mods" << std::endl;

	for (auto& plugin : plugins) plugin->Unload();
	FreeLibrary(hModuleCore);

	return realTRenderD3DInterfaceDestructor(_this, _edx, flag);
}

void LoadMods()
{
	for (const auto& entry : fs::directory_iterator(L"Mods"))
	{
		if (entry.path().extension().compare(L".dll") == 0)
		{
			const wchar_t* dll = entry.path().native().c_str();
			HMODULE hModule = LoadLibraryW(dll);

			GetPluginInstance getPluginInstance = (GetPluginInstance)GetProcAddress(hModule, "GetPluginInstance");

			if (getPluginInstance != nullptr)
			{
				IPlugin* plugin = getPluginInstance();
				
				if (plugin)
				{
					plugin->hModule = hModule;
					bool loaded = plugin->Initialize();
					
					if (loaded)
					{
						std::wcout << L"[Modloader] Successfuly loaded mod " << plugin->GetName() << " [" << plugin->GetVersion() << "]" << std::endl;
						plugins.push_back(plugin);
					}
					else
					{
						std::wcout << L"[Modloader] Unable to load mod " << plugin->GetName() << " [" << plugin->GetVersion() << "]" << std::endl;
					}
				}
				else
				{
					std::cout << "[Modloader] Unable to get an instance of the plugin: " << dll << std::endl;
				}
			}
			else
			{
				std::wcout << L"[Modloader] Loaded unknown mod " << dll << std::endl;
			}
		}
	}
}

DWORD APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
#if _DEBUG
		AllocConsole();
		FILE* fDummy;
		freopen_s(&fDummy, "CONOUT$", "w", stdout);
#endif
		hModuleCore = hModule;
		DetourRestoreAfterWith();
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		Attach(TRenderD3DInterfaceDestructor);
		DetourTransactionCommit();

		LoadMods();
		return true;
	default:
		return false;
	}
}