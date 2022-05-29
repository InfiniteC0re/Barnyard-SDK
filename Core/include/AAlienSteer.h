#pragma once
#include "ToshiClass.h"
#include "AUnitProxy.h"
#include "ASteerKinematics3D.h"

class AAlienSteer : public ToshiClass
{
public:
	void* m_vTable;
	AUnitProxy* m_pUnitProxy;
	int unk1;
	int unk2;
	int unk3;
	int unk4;
	int* unk5;
	int* unk6;
	ASteerKinematics3D* m_pKinematics3D;

	bool SetMovementSpeed(Vector4* pSpeed);
	int Jump();
};

