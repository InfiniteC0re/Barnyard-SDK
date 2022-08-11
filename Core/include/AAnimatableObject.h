#pragma once
#include "TObject.h"
#include "TPString8.h"

class AAnimatableObject : public TObject
{
public:
	bool PlayAnimation(TPString8** name)
	{
		typedef bool(__thiscall* t_PlayAnimation)(AAnimatableObject*, TPString8**);
		static t_PlayAnimation PlayAnimation = (t_PlayAnimation)0x0057d130;

		return PlayAnimation(this, name);
	}

	bool IsAnimation(TPString8** name)
	{
		typedef bool(__thiscall* t_IsAnimation)(AAnimatableObject*, TPString8**, float);
		static t_IsAnimation IsAnimation = (t_IsAnimation)0x0057b630;

		return IsAnimation(this, name, 0);
	}
};