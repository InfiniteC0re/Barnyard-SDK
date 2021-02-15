#include <iostream>
#include <Windows.h>

#include "Engine\ALocalization.h"
#include "Engine\AHUD.h"
#include "Game\Player.h"
#include "Structs\Vector3.h"
#include <fstream>

DWORD WINAPI MainThread(LPVOID lpArgs)
{
	// Open console
	AllocConsole();
	FILE* fp;

	freopen_s(&fp, "CONOUT$", "w", stdout);

	uintptr_t baseAddr = (uintptr_t)GetModuleHandle(NULL);

	Engine::ALocalization ALocalization(baseAddr);
	Engine::AHUD AHUD(baseAddr);
	Game::Player Player(baseAddr);

	while (!GetAsyncKeyState(VK_END))
	{
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			AHUD.OpenMainMenu(); // ONLY WHEN IT'S ALREADY OPENED
		}
		else if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			// DUMP ALL STRINGS IN A FILE (LOCATED IN FOLDER WITH Barnyard.exe)

			const char* fn = "dump.txt";
			std::ofstream dumpFile;
			dumpFile.open(fn);

			for (int i = 0; i < 3429; i++)
			{
				dumpFile << ALocalization.GetString(i) << std::endl;
			}

			dumpFile.close();

			std::cout << "All localization data saved in " << fn << std::endl;
		}

		Sleep(10);
	}

	FreeLibraryAndExitThread((HMODULE)lpArgs, 0);
	return 0;
}

DWORD APIENTRY DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		CreateThread(NULL, NULL, MainThread, instance, NULL, NULL);

		return TRUE;
	}

	return FALSE;
}