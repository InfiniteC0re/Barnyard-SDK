#pragma once
#include "../Steer/ASteer.h"
#include "../AAnimatableObject.h"

class AUnit : public TObject
{
public:
	int m_Unk1;
	int m_Unk2;
	int m_Unk3;
	int m_Unk4;
	void* m_Unk5;
	int m_Unk6;
	ASteer* m_pSteer;
	int m_Unk8;
	int m_Unk9;
	int m_Unk10;
	int m_Unk11;
	int m_Unk12;
	int m_Unk13;
	int m_Unk14;
	int m_Unk15;

	/* AUnit has 15 DWORD fields */

	bool AssignSteer(TObjectInfo* pSteerInfo)
	{
		typedef bool(__thiscall* t_AssignSteerImpl)(AUnit*, TObjectInfo*);
		static t_AssignSteerImpl AssignSteerImpl = (t_AssignSteerImpl)0x00413140;

		return AssignSteerImpl(this, pSteerInfo);
	}

	float GetDistanceSqToUnit(AUnit* target)
	{
		typedef float(__thiscall* t_GetDistanceSqToUnit)(AUnit*, AUnit*);
		static t_GetDistanceSqToUnit GetDistanceSqToUnitImpl = (t_GetDistanceSqToUnit)0x00412fc0;

		return GetDistanceSqToUnitImpl(this, target);
	}
};

class AVisibleUnit : public AUnit
{
public:
	int m_Unk1;
	int m_Unk2;
	int m_Unk3;
	int m_Unk4;
	int m_Unk5;
	int m_Unk6;
	int m_Unk7;
	int m_Unk8;
	int m_Unk9;
	int m_Unk10;
	int m_Unk11;
	int m_Unk12;
	int m_Unk13;
	int m_Unk14;
	int m_Unk15;
	int m_Unk16;
	int m_Unk17;
	int m_Unk18;
	int m_Unk19;
	int m_Unk20;
	int m_Unk21;

	bool IsVisible()
	{
		typedef bool(__thiscall* t_IsVisible)(AVisibleUnit*);
		static t_IsVisible IsVisible = (t_IsVisible)0x004b9420;

		return IsVisible(this);
	}

	void SetVisible(bool state)
	{
		typedef void(__thiscall* t_SetVisible)(AVisibleUnit*, bool);
		static t_SetVisible SetVisible = (t_SetVisible)0x00414f80;

		SetVisible(this, state);
	}
};

class AAnimatedUnit : public AVisibleUnit
{
public:
	AAnimatableObject* m_AnimatableObject;
};

class ACharacterUnit : public AAnimatedUnit
{
public:
	int m_Unk1;
	int m_Unk2;

	void SetDesireWalk()
	{
		typedef void(__thiscall* t_SetDesireWalk)(ACharacterUnit*);
		static t_SetDesireWalk SetDesireWalkImpl = (t_SetDesireWalk)0x00417010;

		SetDesireWalkImpl(this);
	}

	void SetDesireRun()
	{
		typedef void(__thiscall* t_SetDesireRun)(ACharacterUnit*);
		static t_SetDesireRun SetDesireRunImpl = (t_SetDesireRun)0x00417030;

		SetDesireRunImpl(this);
	}

	void SetDesiredSpeed(float speed)
	{
		typedef void(__thiscall* t_SetDesiredSpeed)(ACharacterUnit*, float);
		static t_SetDesiredSpeed SetDesiredSpeedImpl = (t_SetDesiredSpeed)0x00417050;

		SetDesiredSpeedImpl(this, speed);
	}

	bool StartConversationWithTarget()
	{
		typedef bool(__thiscall* t_StartConversationWithTarget)(ACharacterUnit*);
		static t_StartConversationWithTarget StartConversationWithTargetImpl = (t_StartConversationWithTarget)0x00417970;

		return StartConversationWithTargetImpl(this);
	}
};