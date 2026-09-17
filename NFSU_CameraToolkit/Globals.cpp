#include <vector>

#include "Globals.h"
#include "IniReader/IniReader.h"
#include "IniWrapper.h"
#include "Utilities.h"
#include "UI.h"
#include "Game.h"

namespace Globals
{
	bool PressedKeys[0xFF];
	std::vector<int> HotKeys;

	Hotkeys HK;
	CameraSettings Settings;

	void SetKeyPressed(int key, bool value)
	{
		if (key > 0 && key < 0xFF)
		{
			PressedKeys[key] = value;
		}
	}

	bool IsKeyPressed(int key)
	{
		if (key >= 0 && key < 0xFF)
		{
			return PressedKeys[key];
		}

		return false;
	}

	bool IsHotKey(int key)
	{
		return Utilities::Contains(HotKeys, key);
	}

	bool firstEnable = true;
	void ToggleCamOverride()
	{
		if (firstEnable)
		{
			gCameraManager.Reset();
			firstEnable = false;
		}

		CameraOverride = !CameraOverride;
	}

	void ToggleDrawUI()
	{
		Globals::DrawUI = !Globals::DrawUI;
	}

	void SetUseMouseToRotate()
	{
		Ini::SetBool("GENERAL", "UseMouseToRotate", Globals::Settings.UseMouseToRotate);
	}
}

void SetHotKey(int& dest, const char* name)
{
	int val = Ini::GetInt("HOTKEYS", name, 0);
	dest = val;
	Globals::HotKeys.push_back(val);
}

void InitGlobals()
{
	Globals::Settings.MoveSpeed = Ini::GetFloat("CAMERA_SETTINGS", "MoveSpeed", 10.0f);
	Globals::Settings.RotationSpeed = Ini::GetFloat("CAMERA_SETTINGS", "RotationSpeed", 10.0f);
	Globals::Settings.RollSpeed = Ini::GetFloat("CAMERA_SETTINGS", "RollSpeed", 10.0f);
	Globals::Settings.FovSpeed = Ini::GetFloat("CAMERA_SETTINGS", "FovSpeed", 10.0f);
	Globals::Settings.UseMouseToRotate = Ini::GetBool("GENERAL", "UseMouseToRotate", true);
	Globals::Settings.GamepadInvert = Ini::GetBool("GENERAL", "GamepadInvert", true);
	Globals::UIScale = Ini::GetFloat("GENERAL", "UIScale", 1.0);

	SetHotKey(Globals::HK.ToggleUI, "ToggleUI");
	SetHotKey(Globals::HK.ToggleCamera, "ToggleCamera");
	SetHotKey(Globals::HK.ToggleHUD, "ToggleHUD");
	SetHotKey(Globals::HK.ExitTool, "ExitTool");
	SetHotKey(Globals::HK.ToggleMouseInput, "ToggleMouseInput");

	SetHotKey(Globals::HK.PauseGame, "PauseGame");

	SetHotKey(Globals::HK.MoveCameraUp, "MoveCameraUp");
	SetHotKey(Globals::HK.MoveCameraDown, "MoveCameraDown");
	SetHotKey(Globals::HK.MoveCameraLeft, "MoveCameraLeft");
	SetHotKey(Globals::HK.MoveCameraRight, "MoveCameraRight");

	SetHotKey(Globals::HK.MoveCameraForward, "MoveCameraForward");
	SetHotKey(Globals::HK.MoveCameraBackward, "MoveCameraBackward");

	SetHotKey(Globals::HK.RotateCameraLeft, "RotateCameraLeft");
	SetHotKey(Globals::HK.RotateCameraRight, "RotateCameraRight");
	SetHotKey(Globals::HK.RotateCameraUp, "RotateCameraUp");
	SetHotKey(Globals::HK.RotateCameraDown, "RotateCameraDown");

	SetHotKey(Globals::HK.RollCameraLeft, "RollCameraLeft");
	SetHotKey(Globals::HK.RollCameraRight, "RollCameraRight");

	SetHotKey(Globals::HK.IncreaseFOV, "IncreaseFOV");
	SetHotKey(Globals::HK.DecreaseFOV, "DecreaseFOV");

	SetHotKey(Globals::HK.AddCameraNode, "AddCameraNode");
	SetHotKey(Globals::HK.PlayTrack, "PlayTrack");
	SetHotKey(Globals::HK.StopTrack, "StopTrack");

	SetHotKey(Globals::HK.ToggleChecked, "ToggleChecked");
	SetHotKey(Globals::HK.ToggleHood, "ToggleHood");
	SetHotKey(Globals::HK.ToggleLeftDoor, "ToggleLeftDoor");
	SetHotKey(Globals::HK.ToggleRightDoor, "ToggleRightDoor");
	SetHotKey(Globals::HK.ToggleTrunk, "ToggleTrunk");
	SetHotKey(Globals::HK.ToggleLeftWindow, "ToggleLeftWindow");
	SetHotKey(Globals::HK.ToggleRightWindow, "ToggleRightWindow");
}