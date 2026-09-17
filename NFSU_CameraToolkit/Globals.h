#pragma once
#include <Windows.h>
#include <vector>

#include "CameraManager.h"
#include "TrackPlayer.h"
#include "Gamepad.h"
#include "Game.h"

inline Gamepad g_Gamepad;
inline POINT WindowSize;

struct Hotkeys
{
	int ToggleUI;
	int ToggleCamera;
	int PauseGame;
	int ToggleHUD;
	int ToggleMouseInput;
	int ExitTool;

	int MoveCameraUp;
	int MoveCameraDown;
	int MoveCameraLeft;
	int MoveCameraRight;
	int MoveCameraForward;
	int MoveCameraBackward;

	int RollCameraLeft;
	int RollCameraRight;

	int RotateCameraLeft;
	int RotateCameraRight;
	int RotateCameraUp;
	int RotateCameraDown;

	int IncreaseFOV;
	int DecreaseFOV;

	int AddCameraNode;
	int PlayTrack;
	int StopTrack;

	int ToggleChecked;
	int ToggleHood;
	int ToggleLeftDoor;
	int ToggleRightDoor;
	int ToggleTrunk;
	int ToggleLeftWindow;
	int ToggleRightWindow;
};

struct CameraSettings
{
	float MoveSpeed;
	float RotationSpeed;
	float RollSpeed;
	float FovSpeed;

	bool UseMouseToRotate;
	bool FovOverride = true;
	bool LodOverride = true;
	bool GamepadInvert = false;
	bool GamepadInput = false;
};

namespace Globals
{
	static const char* ToolName = "NFSU - Camera ToolKit v1.0";

	inline HINSTANCE DllHandle;
	inline HWND Window = NULL;
	inline bool ShutDown = false;
	inline XMMATRIX CamBackup;

	inline bool DrawUI = false;
	inline bool CameraOverride = false;

	inline float UIScale = 1.0f;
	extern Hotkeys HK;
	extern CameraSettings Settings;

	inline CameraMode gCameraMode;
	inline CameraManager gCameraManager;
	inline TrackPlayer gTrackPlayer;

	void SetKeyPressed(int key, bool value);
	bool IsKeyPressed(int key);
	bool IsHotKey(int key);

	void ToggleCamOverride();
	void SetUseMouseToRotate();
	void ToggleDrawUI();
}

void InitGlobals();