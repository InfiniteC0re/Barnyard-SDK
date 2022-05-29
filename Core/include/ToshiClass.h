#pragma once

#define TCAST(type, val) static_cast<type>(val)

struct ToshiClassInfo
{
	char* className;
	void* pEasyCtor;
	void* pCtor;
	void* pScriptingAPI;
	void* pEmptyFunc;
	int* unk1;
	ToshiClassInfo* pParent;
	ToshiClassInfo* pAttached;
	unsigned int unk2;
	unsigned int size;
	unsigned int unk3;
	int unk4;
};

struct ToshiClassVTable
{
	ToshiClassInfo* (*GetTClassInfo)();
};

class ToshiClass
{
public:
	ToshiClassVTable* GetVTable();
	void* FindAttachedToshiClass(ToshiClassInfo* pClassInfo);
	static ToshiClassInfo* FindByName(const char* name, ToshiClassInfo* pClassInfo);
};