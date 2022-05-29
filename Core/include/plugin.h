#pragma once

class IPlugin
{
public:
	HMODULE hModule;
	static IPlugin* Instance;

	virtual const char* GetName() const = 0;
	virtual const char* GetVersion() const = 0;
	virtual bool Initialize() = 0;
	virtual bool Unload() = 0;
};