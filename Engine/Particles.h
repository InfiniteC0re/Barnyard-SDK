#pragma once
#include <Windows.h>
#include "../Structs/Vector4.h"
#include "../Structs/ParticleSystem.h"
#include <fstream>

namespace Engine
{
	class Particles
	{
	private:
		typedef int(__stdcall* _GetParticlesByNameA)(const char* particlesName);
		typedef const char**(__thiscall* _GetParticlesNameByIDA)(void* pParticleList, int particlesID);
		typedef ParticleSystem*(__thiscall* _SpawnParticleSystemA)(void* pGameResources, int particlesID, Vector4* coordinates, Vector4* rotationVector);
		typedef void(__thiscall* _SetDirectionA)(ParticleSystem* pPSystem, Vector4* vec1, Vector4* upVector);
		typedef void(__thiscall* _SetGlobalRotationA)(ParticleSystem* particleSystem, float angle);
		typedef ParticleSystem*(__thiscall* _CreateParticleSystemA)(void* pParticleList, const char** pParticles);

		typedef void(__thiscall* _FUN_006c8120)(ParticleSystem* particleSystem);
		typedef void(__thiscall* _FUN_006c80e0)(ParticleSystem* particleSystem, int* p);

		_GetParticlesByNameA _GetParticlesByName;
		_GetParticlesNameByIDA _GetParticlesNameByID;
		_SpawnParticleSystemA _SpawnParticleSystem;
		_CreateParticleSystemA _CreateParticleSystem;
	public:
		_FUN_006c8120 FUN_006c8120;
		_FUN_006c80e0 FUN_006c80e0;

		_SetDirectionA SetDirection;
		_SetGlobalRotationA SetGlobalRotation;
		ParticleSystem* SpawnParticleSystem(const char* particlesName, Vector4 coordinates);
		ParticleSystem* CreateParticleSystem(const char* particlesName);
		const char* GetParticlesNameByID(int id);
		Particles();
		Particles(uintptr_t base);
	};
}


