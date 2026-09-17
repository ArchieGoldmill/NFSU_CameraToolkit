#pragma once

void* GetCarByDriverNumber(int driverNumber)
{
	void* result;

	__asm {
		mov edi, driverNumber
		mov eax, 0x0041FFE0;
		call eax;
		mov result, eax
	}

	return result;
}

struct CarState
{
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
	int field_20;
	int field_24;
	int field_28;
	int field_2C;
	int field_30;
	int field_34;
	int field_38;
	int field_3C;
	int field_40;
	int field_44;
	int field_48;
	int field_4C;
	XMVECTOR Position;
	XMVECTOR unk1;
	XMVECTOR unk2;
	XMVECTOR unk3;
	XMMATRIX Matrix;

	inline static CarState* GetPlayer()
	{
		auto playerCarNumber = (int*)0x0078A410;
		return (CarState*)GetCarByDriverNumber(playerCarNumber[0]);
	}
};