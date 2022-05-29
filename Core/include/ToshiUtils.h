#pragma once

struct DDSTextureData
{
	int m_textureDataSize;
	char* m_pTextureData;
	int* m_unk1;
	int* m_unk2;
	int m_width;
	int m_height;
	int m_ddsFlag1;
	int m_one;
	int m_unk4;
	int m_unk5;
	int m_unk6;
	int m_unk7;
};

class ToshiUtils
{
public:
	static int* TAlloc(int size, void* block = nullptr);
	static bool TFree(void* object);
	static void MakeVector(void* param_1, int structSize, int count, int structMaker, int param_5);
	static void CopyString(void* dest, char* src);
	static void CloseTRB(void* trb);
	static void* DoesTextureExist(const char* name, int f1, int f2);
	static int SaveDDSTexture(DDSTextureData* pTextureData);
};

