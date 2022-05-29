#pragma once
#include "ToshiClass.h"
#include "AAlienSteer.h"

class AUnitPlayer : public ToshiClass
{
public:
	void* m_vTable;
	int unk1;
	int unk2;
	int unk3;
	int unk4;
	AUnitPlayer* m_pAttachedToBike;
	int unk5;
	AAlienSteer* m_pSteer;

	void SetSkin(int index);
};