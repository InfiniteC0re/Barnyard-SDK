#pragma once
#include "../AUnitProxy.h"
#include "../TPString8.h"
#include "../Math.h"

class AUnit;

class ASteer : public TObject
{
public:
	virtual void VUnk1() = 0;
	virtual void VUnk2() = 0;
	virtual void VUnk3() = 0;
	virtual void VUnk4() = 0;
	virtual void VUnk5() = 0;
	virtual void VUnk6() = 0;
	virtual void VUnk7() = 0;
	virtual void VUnk8() = 0;
	virtual void VUnk9() = 0;
	virtual void VUnk10() = 0;
	virtual void VUnk11() = 0;
	virtual void VUnk12() = 0;
	virtual void VUnk13() = 0;
	virtual void VUnk14() = 0;
	virtual void VUnk15() = 0;
	virtual void VUnk16() = 0;
	virtual void VUnk17() = 0;
	virtual void VUnk18() = 0;
	virtual void VUnk19() = 0;
	virtual void StandStill() = 0;
	virtual void VUnk21() = 0;
	virtual void VUnk22() = 0;
	virtual void VUnk23() = 0;
	virtual void FaceUnit(AUnit*) = 0;
	virtual void VUnk25() = 0;
	virtual void CancelCurrentCommand() = 0;

public:
	AUnitProxy* m_pUnitProxy;
	int m_Unk1;
	int m_Unk2;
	int m_Unk3;
	int m_Unk4;
};

class ACommonSteer : public ASteer
{

};

class AInteriorSteer : public ACommonSteer
{
public:
	void Command_SeekPlayer()
	{
		typedef void(__thiscall* t_SeekPlayer)(AInteriorSteer*);
		static t_SeekPlayer SeekPlayerImpl = (t_SeekPlayer)0x00561d10;

		SeekPlayerImpl(this);
	}

	bool Command_SnapConnectNamedNetwork(TPString8* name)
	{
		typedef bool(__thiscall* t_SnapConnectNamedNetwork)(AInteriorSteer*, TPString8* name, bool flag);
		static t_SnapConnectNamedNetwork SnapConnectNamedNetworkImpl = (t_SnapConnectNamedNetwork)0x00563cb0;

		return SnapConnectNamedNetworkImpl(this, name, true);
	}

	void Command_PatrolUnitPoint(AUnit* unit, float distance, float param2)
	{
		typedef void(__thiscall* t_PatrolUnitPoint)(AInteriorSteer*, AUnit*, float, float);
		static t_PatrolUnitPoint PatrolUnitPointImpl = (t_PatrolUnitPoint)0x005644b0;

		PatrolUnitPointImpl(this, unit, distance, param2);
	}

	void Command_PatrolUnitPointKeepDist(AUnit* unit, float distance)
	{
		typedef void(__thiscall* t_PatrolUnitPointKeepDist)(AInteriorSteer*, AUnit*, float);
		static t_PatrolUnitPointKeepDist PatrolUnitPointKeepDistImpl = (t_PatrolUnitPointKeepDist)0x00564500;

		PatrolUnitPointKeepDistImpl(this, unit, distance);
	}

	void Command_SeekUnitPointOver(AUnit* unit)
	{
		typedef void(__thiscall* t_SeekUnitPointOver)(AInteriorSteer*, AUnit*);
		static t_SeekUnitPointOver SeekUnitPointOverImpl = (t_SeekUnitPointOver)0x00564120;

		SeekUnitPointOverImpl(this, unit);
	}

	bool Command_SeekPointXYZ(const Vector3& pos)
	{
		typedef bool(__thiscall* t_SeekPointXYZ)(AInteriorSteer*, Vector3);
		static t_SeekPointXYZ SeekPointXYZ = (t_SeekPointXYZ)0x00563e10;

		return SeekPointXYZ(this, pos);
	}

	bool Command_AvoidUnit(AUnit* unit, float param1, bool param2)
	{
		typedef bool(__thiscall* t_AvoidUnit)(AInteriorSteer*, AUnit*, float, bool);
		static t_AvoidUnit AvoidUnit = (t_AvoidUnit)0x00566420;

		return AvoidUnit(this, unit, param1, param2);
	}

	void Command_StateStill()
	{
		typedef void(__thiscall* t_StateStill)(AInteriorSteer*);
		static t_StateStill StateStill = (t_StateStill)0x00561ad0;

		StateStill(this);
	}

	bool Command_EndStateStill()
	{
		typedef bool(__thiscall* t_EndStateStill)(AInteriorSteer*);
		static t_EndStateStill EndStateStill = (t_EndStateStill)0x00561af0;

		return EndStateStill(this);
	}
};

class APlayerSteer : public ACommonSteer
{

};