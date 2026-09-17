#pragma once
#include "Globals.h"

void UpdateGamepadInputs()
{
	g_Gamepad.Update();

	if (!Globals::Settings.GamepadInput)
	{
		if (g_Gamepad.IsPressedOnce(XINPUT_GAMEPAD_A))
		{
			Globals::gTrackPlayer.Play();
		}

		if (g_Gamepad.IsPressedOnce(XINPUT_GAMEPAD_B))
		{
			Globals::gTrackPlayer.Stop();
		}

		if (g_Gamepad.IsPressedOnce(XINPUT_GAMEPAD_X))
		{
			Globals::gTrackPlayer.AddNode();
		}
	}
}

POINT LastMousePos;
bool LastFocus = false;
void UpdateMouseInputs()
{
	if (Globals::CameraOverride && !Globals::DrawUI && Globals::Settings.UseMouseToRotate)
	{
		bool focused = Game::InFocus();
		if (focused != LastFocus)
		{
			GetCursorPos(&LastMousePos);
			LastFocus = focused;
		}

		POINT pos;
		GetCursorPos(&pos);

		Globals::gCameraManager.UpdateMouse(-pos.x + LastMousePos.x, -pos.y + LastMousePos.y, true);

		LastMousePos = pos;

		if (pos.x == 0 || pos.x >= WindowSize.x - 1 || pos.y == 0 || pos.y >= WindowSize.y - 1)
		{
			SetCursorPos(WindowSize.x / 2, WindowSize.y / 2);
			LastMousePos = POINT(WindowSize.x / 2, WindowSize.y / 2);
		}
	}
	else
	{
		GetCursorPos(&LastMousePos);
	}
}

bool KeyPressedOnce(int wParam, int hk)
{
	return wParam == hk && !Globals::IsKeyPressed(hk);
}

POINT LastMousePosUI;
void HandleKeyDownInput(int wParam)
{
	if (wParam < 1 || wParam > 0xFF)
	{
		return;
	}

	if (KeyPressedOnce(wParam, Globals::HK.ToggleUI))
	{
		Globals::ToggleDrawUI();
		if (Globals::DrawUI && Globals::CameraOverride)
		{
			SetCursorPos(LastMousePosUI.x, LastMousePosUI.y);
		}
	}

	if (!Globals::gTrackPlayer.IsPlaying())
	{
		if (KeyPressedOnce(wParam, Globals::HK.ToggleCamera))
		{
			Globals::ToggleCamOverride();
		}

		if (KeyPressedOnce(wParam, Globals::HK.PauseGame))
		{
			if (Game::Speed != 1)
			{
				Game::Speed = 1;
			}
			else
			{
				Game::Speed = 0.01;
			}
		}

		if (!Globals::DrawUI)
		{
			if (KeyPressedOnce(wParam, Globals::HK.AddCameraNode))
			{
				Globals::gTrackPlayer.AddNode();
			}

			if (KeyPressedOnce(wParam, Globals::HK.PlayTrack))
			{
				Globals::gTrackPlayer.Play();
			}
		}
	}

	if (KeyPressedOnce(wParam, Globals::HK.StopTrack))
	{
		Globals::gTrackPlayer.Stop();
	}

	if (KeyPressedOnce(wParam, Globals::HK.ToggleMouseInput))
	{
		Globals::Settings.UseMouseToRotate = !Globals::Settings.UseMouseToRotate;
		Globals::SetUseMouseToRotate();
	}

	if (KeyPressedOnce(wParam, Globals::HK.ToggleHUD))
	{
		Game::DrawFE ^= true;
	}

	Globals::SetKeyPressed(wParam, true);
}

void HandleKeyUpInput(int wParam)
{
	Globals::SetKeyPressed(wParam, false);
}