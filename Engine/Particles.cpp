#include "Particles.h"
#include <iostream>

Engine::Particles::Particles() { }

Engine::Particles::Particles(uintptr_t base)
{
	_GetParticlesByName = (_GetParticlesByNameA)(base + 0x00250fb0);
	_GetParticlesNameByID = (_GetParticlesNameByIDA)(base + 0x00274f90);
	_SpawnParticleSystem = (_SpawnParticleSystemA)(base + 0x002513b0);
	_CreateParticleSystem = (_CreateParticleSystemA)(base + 0x00274df0);
	SetDirection = (_SetDirectionA)(base + 0x002c8970);
	SetGlobalRotation = (_SetGlobalRotationA)(base + 0x002c8840);

	FUN_006c8120 = (_FUN_006c8120)(base + 0x002c8120);
	FUN_006c80e0 = (_FUN_006c80e0)(base + 0x002c80e0);
}

ParticleSystem* Engine::Particles::SpawnParticleSystem(const char* particlesName, Vector4 coordinates)
{
	int particlesID = _GetParticlesByName(particlesName);

	if (particlesID != -1)
	{
		Vector4 UpVector;
		UpVector.y = -1;

		return _SpawnParticleSystem(*(void**)0x007b5ea4, particlesID, &coordinates, &UpVector);
	}

	return (ParticleSystem*)0x0;
}

const char* Engine::Particles::GetParticlesNameByID(int id)
{
	const char** particlesName = _GetParticlesNameByID(*(void**)((*(int*)0x007B5EA4) + 0x24), id);

	return *particlesName;
}

ParticleSystem* Engine::Particles::CreateParticleSystem(const char* particlesName)
{
	int particlesID = _GetParticlesByName(particlesName);

	if (particlesID != -1)
	{
		ParticleSystem* particleSystem = _CreateParticleSystem(*(void**)((*(int*)0x007B5EA4) + 0x24), &particlesName);

		return particleSystem;
	}

	return (ParticleSystem*)0x0;
}
