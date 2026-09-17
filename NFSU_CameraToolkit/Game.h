#pragma once
#include <DirectXMath.h>
#include <d3d9.h>

using namespace DirectX;

typedef unsigned int Hash;

namespace Game
{
	inline auto Device = (LPDIRECT3DDEVICE9*)0x00870974;
	inline auto& Window = *(HWND*)0x00870990;
	inline auto DrawFE = true;
	inline auto ForceRain = (bool*)0x008A1D38;
	inline auto& Speed = *(float*)0x007A5730;
	inline auto FrontSteerAngle = (float*)0x007F45A4;
	inline auto& CarLod = *(int*)0x00802728;
	inline auto& TireLOD = *(int*)0x0080272C;
	inline auto& State = *(int*)0x008654A4;
	inline auto NotInFocus = (bool*)0x008709E0;
	inline float& DeltaTime = *(float*)0x00865198;

	inline auto eCreateLookAtMatrix = (int(__cdecl*)(void*, void*, void*, void*))0x005BA9A0;

	inline bool InFocus()
	{
		return !*Game::NotInFocus;
	}

	struct __declspec(align(0x10)) CameraParams
	{
		XMMATRIX Matrix;
		XMVECTOR Position;
		XMVECTOR Direction;
		int field_60;
		int field_64;
		int field_68;
		int field_6C;
		int field_70;
		int field_74;
		int field_78;
		int field_7C;
		int field_80;
		int field_84;
		int field_88;
		int field_8C;
		int field_90;
		int field_94;
		int field_98;
		int field_9C;
		int field_A0;
		int field_A4;
		int field_A8;
		int field_AC;
		int field_B0;
		int field_B4;
		int field_B8;
		int field_BC;
		int field_C0;
		__int16 horizontal_fov;
	};

	struct __declspec(align(0x10)) Camera
	{
		CameraParams CurrentKey;
	};

	inline auto PlayerCamera = (Camera*)0x008760e0;
}