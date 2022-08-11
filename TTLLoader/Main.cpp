#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <Windows.h>
#include "detours.h"
#include "include/ToshiUtils.h"
#include "include/plugin.h"

#include <d3d8.h>
#include <map>

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
#define DEBUG_PRINT(args) printf("[TTLLoader] "); printf args
#else
#define DEBUG_PRINT(args)
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

std::map<std::string, DDSTextureData*> loadedTextures;

stdCallHook(TFree, 0x006b4a20, bool, void* region)
{
	// all dds data is stored using malloc so we don't have to use Toshi's free method
	// 0x20534444 is the DDS magic number
	if (region && *(int*)region == 0x20534444)
	{
		free(region);
		return true;
	}
	else return realTFree(region);
}

thisCallHook(GetDDSTextureFromTTL, 0x00615d20, int*, bool, TTLTexturesData* ttlSymb)
{
	int* pTexturesInfo = ToshiUtils::TAlloc(ttlSymb->count * 0x10 + 4);
	TextureInformation* pVector = (TextureInformation*)(pTexturesInfo + 1);
	*pTexturesInfo = ttlSymb->count;

	ToshiUtils::MakeVector(pVector, sizeof(TextureInformation), ttlSymb->count, 0x00615ca0, 0x00615c90);
	*(TextureInformation**)(_this + 5) = pVector;
	*(TextureInformation**)(_this + 6) = pVector;
	*(int*)(_this + 7) = ttlSymb->count;
	
	DEBUG_PRINT(("Loading ttl with %d textures\n", ttlSymb->count));

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

			int nameLen = strlen(texInfo->m_texName);
			char filePath[256];
			filePath[0] = 0;

			strcat(filePath, "Data\\Textures\\");
			strncat(filePath, texInfo->m_texName, nameLen - 3);
			strcat(filePath, "dds");

			FILE* patchFile = fopen(filePath, "rb");

			if (patchFile)
			{
				fseek(patchFile, 0, SEEK_END);
				size_t size = ftell(patchFile);
				fseek(patchFile, 0, SEEK_SET);
				texData->m_textureDataSize = (int)size;

				// not using Toshi's alloc function to avoid memory block overflow
				texData->m_pTextureData = (char*)malloc(size);
				if (texData->m_pTextureData) fread(texData->m_pTextureData, size, 1, patchFile);

				DEBUG_PRINT(("Loaded texture %s\n", filePath));
				fclose(patchFile);
			}
			else
			{
				// not using Toshi's alloc function to avoid memory block overflow
				texData->m_pTextureData = (char*)malloc(texInfo->m_texSize);
				
				if (texData->m_pTextureData != 0)
				{
					memcpy(texData->m_pTextureData, texInfo->m_texData, texInfo->m_texSize);
					texData->m_textureDataSize = texInfo->m_texSize;

					// dumping textures
					std::ofstream outFile(filePath, std::ios::binary | std::ios::out);
					outFile.write(texInfo->m_texData, texInfo->m_texSize);
					outFile.close();

					DEBUG_PRINT(("Dumped texture %s\n", filePath));
				}
			}

			ToshiUtils::SaveDDSTexture(texData);

			(texData->m_texture)->GetLevelCount();
			*(DDSTextureData**)(&pVector[i].m_flag) = texData;

			// save it to the map
			loadedTextures[filePath] = texData;
		}
		else
		{
			*(DDSTextureData**)(&pVector[i].m_flag) = nullptr;
		}
	}
	
	ToshiUtils::CloseTRB((int*)(_this + 8));
	return true;
}

class TTLMod : public IPlugin
{
	bool Initialize();
	bool Unload();

	const char* GetName() const
	{
		return "TTL Loader";
	}

	const char* GetVersion() const
	{
		return "1.0.0";
	}
};

IPlugin* TTLMod::Instance = new TTLMod;

DWORD APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved)
{
	return reason == DLL_PROCESS_ATTACH;
}

DWORD WINAPI MainThread(HMODULE hModule)
{
	while (true)
	{
		if (GetAsyncKeyState(VK_F11) & 1)
		{
			for (auto const& x : loadedTextures)
			{
				const std::string& filepath = x.first;
				const DDSTextureData* dds = x.second;
				
				D3DLOCKED_RECT lockedRect;
				printf("Ptr: %p\n", dds->m_texture);
				//(*dds->m_texture)->GetLevelCount();
				//dds->m_texture->LockRect(0, &lockedRect, nullptr, 0);

				/*unsigned short* data = (unsigned short*)lockedRect.pBits;
				for (int i = 0; i < dds->m_textureDataSize; i++)
				{
					data[i] = 255;
				}*/
			}
		}

		Sleep(30);
	}
	return 0;
}

bool TTLMod::Initialize()
{
	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	Attach(TFree);
	Attach(GetDDSTextureFromTTL);
	DetourTransactionCommit();

	CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, 0));

	return true;
}

bool TTLMod::Unload()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	Dettach(TFree);
	Dettach(GetDDSTextureFromTTL);
	DetourTransactionCommit();

	delete TTLMod::Instance;
	FreeLibrary(this->hModule);
	DEBUG_PRINT(("Unloaded TTL Loader..."));

	return true;
}

extern "C" __declspec(dllexport) IPlugin* GetPluginInstance()
{
	return TTLMod::Instance;
}