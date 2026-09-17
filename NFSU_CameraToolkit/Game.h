#pragma once
#include <DirectXMath.h>
#include <d3d9.h>

using namespace DirectX;

typedef unsigned int Hash;

namespace Game
{
	inline auto Device = (LPDIRECT3DDEVICE9*)0x0073636C;
	inline auto& Window = *(HWND*)0x00736380;
	inline auto& DrawFE = *(bool*)0x006F9228;
	inline auto& Speed = *(float*)0x006B7994;
	inline auto& CarLod = *(int*)0x006F1B30;
	inline auto& TireLOD = *(int*)0x006F1B34;
	inline auto& State = *(int*)0x0077A920;
	inline auto NotInFocus = (bool*)0x007363B6;
	inline float& DeltaTime = *(float*)0x0073AD38;

	inline void* LookAtTarget = (void*)0x0040B780;
	inline void __declspec(naked) eCreateLookAtMatrix(void*, void*, void*, void*)
	{
		__asm
		{
			push edi;
			push esi;
			mov esi, [esp + 0Ch];
			mov edi, [esp + 10h];
			mov eax, [esp + 14h];
			push dword ptr[esp + 18h];
			call dword ptr[LookAtTarget];
			add esp, 4;
			pop esi;
			pop edi;
			ret;
		}
	}

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

	inline auto PlayerCamera = (Camera*)0x0078d270;
}