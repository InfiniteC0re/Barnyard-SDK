#pragma once
#include "../TObject.h"

class AInteractionActivity : public TObject
{
public:
	virtual void ReadProperties() = 0;
	virtual void SetFirstToZero() = 0;
	virtual int GetUnk1() = 0;
	virtual int GetUnk2() = 0;
	virtual int Action(int unused = 0) = 0;
	virtual float SomeFloat() = 0;
	virtual int* GetUnk3() = 0;
	virtual int GetUnk4() = 0;
	virtual bool GetUnk5() = 0;
	virtual bool GetUnk6() = 0;
	virtual bool GetUnk7() = 0;
	virtual bool GetUnk8() = 0;
};