#pragma once
#include "../TObject.h"
#include "../AUnitProxy.h"
#include "../ASteerKinematics3D.h"
#include "ASteer.h"

class AAlienSteer : public ACommonSteer
{
public:
	int* unk5;
	int* unk6;
	ASteerKinematics3D* m_pKinematics3D;

	bool SetMovementSpeed(Vector4* pSpeed);
	int Jump();
};

