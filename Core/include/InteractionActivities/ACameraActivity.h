#pragma once
#include "AInteractionActivity.h"
#include "../TPString8.h"

class ACameraActivityInfo : public AInteractionActivity
{
public:
	inline void SetMode(TPString8* string)
	{
		m_Mode = string;
	}

private:
	int m_Unused;
	TPString8* m_Mode;
};