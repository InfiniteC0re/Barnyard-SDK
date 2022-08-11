#pragma once

#define TCAST(type, val) static_cast<type>(val)

struct TObjectInfo
{
	char* className;
	void* pEasyCtor;
	void* pCtor;
	void* pScriptingAPI;
	void* pEmptyFunc;
	int* unk1;
	TObjectInfo* pParent;
	TObjectInfo* pAttached;
	unsigned int unk2;
	unsigned int size;
	unsigned int unk3;
	int unk4;

	void* Create()
	{
		typedef void* (__thiscall* t_CallTClassEasyCTOR)(TObjectInfo*);
		t_CallTClassEasyCTOR CallTClassEasyCTOR = (t_CallTClassEasyCTOR)0x006bbce0;
		
		return CallTClassEasyCTOR(this);
	}
};

class TObject
{
public:
	virtual TObjectInfo* Info() = 0;
	virtual void Alloc() = 0;
	virtual void Delete(char freeMemory) = 0;

	void* FindAttachedToshiClass(TObjectInfo* pClassInfo);

	static TObjectInfo* Find(const char* name, TObjectInfo* pClassInfo);
};