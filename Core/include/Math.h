#pragma once

struct Vector3
{
	float x, z, y;
};

struct Vector4
{
	float x, z, y, w;
};

class Mat43
{
public:
	static Mat43 Make(Vector4* pos, Vector4* rot, int x, int y, int z, bool flag)
	{
		typedef void(__thiscall* t_MakeMat43)(Mat43*, Vector4* posVec, Vector4* rotVec, int x, int y, int z, bool someFlag);
		static t_MakeMat43 MakeMat43 = (t_MakeMat43)(0x00412ee0);

		Mat43 transform;
		MakeMat43(&transform, pos, rot, x, y, z, flag);

		return transform;
	}

public:
	Vector4 m_Pos;
	Vector4 m_Rot;
	Vector4 m_Unk;
};