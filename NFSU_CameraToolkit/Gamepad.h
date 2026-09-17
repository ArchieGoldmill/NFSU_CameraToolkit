#pragma once
#include <Windows.h>
#include <xinput.h>
#include <DirectXMath.h>

using namespace DirectX;

class Gamepad
{
private:
	int index;
	XINPUT_STATE xStateLast;
	XINPUT_STATE xState;
	bool connected;

public:
	Gamepad()
	{
		this->index = 0;
		this->connected = false;
	}

	void Update()
	{
		this->xStateLast = this->xState;
		ZeroMemory(&this->xState, sizeof(XINPUT_STATE));
		
		this->connected = XInputGetState(this->index, &this->xState) == ERROR_SUCCESS;
	}

	bool IsConnected()
	{
		return this->connected;
	}

	XMFLOAT2 GetLStick()
	{
		auto vec = GetStick(this->xState.Gamepad.sThumbLX, this->xState.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		float div = 100000.0f;
		return { vec.x / div,vec.y / div };
	}

	XMFLOAT2 GetRStick()
	{
		auto vec = GetStick(this->xState.Gamepad.sThumbRX, this->xState.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		float div = 3000.0f;
		return { vec.x / div,vec.y / div };
	}

	float GetLTrigger()
	{
		return this->GetTrigger(this->xState.Gamepad.bLeftTrigger);
	}

	float GetRTrigger()
	{
		return this->GetTrigger(this->xState.Gamepad.bRightTrigger);
	}

	bool IsPressed(int key)
	{
		return this->IsPressed(this->xState, key);
	}

	bool IsPressedOnce(int key)
	{
		return this->IsPressed(this->xState, key) && !this->IsPressed(this->xStateLast, key);
	}

private:
	XMFLOAT2 GetStick(float x, float y, float deadZone)
	{
		float magnitude = sqrt(x * x + y * y);
		if (magnitude > deadZone)
		{
			return { x , -y };
		}

		return { 0,0 };
	}

	float GetTrigger(float t)
	{
		if (t > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			return t / 10000.0f;
		}

		return 0;
	}

	bool IsPressed(XINPUT_STATE state, int key)
	{
		return state.Gamepad.wButtons & key;
	}
};