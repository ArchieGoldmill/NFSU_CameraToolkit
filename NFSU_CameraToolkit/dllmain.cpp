#include <Windows.h>
#include <windowsx.h>
#include <thread>
#include <d3d9.h>

#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "minhook/MinHook.h"
#include "Injector/injector.hpp"
#include "UI.h"
#include "Globals.h"
#include "InputHandle.h"
#include "CarState.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "XInput9_1_0.lib")
#pragma comment(lib, "XInput.lib")

typedef HRESULT(__stdcall* tEndScene)(LPDIRECT3DDEVICE9);
typedef HRESULT(__stdcall* tReset)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);

tEndScene oEndScene;
tReset oReset;
WNDPROC oWNDPROC;
bool initUI = false;
bool wndProcHooked = false;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (Globals::DrawUI)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		{
			return true;
		}
	}

	switch (msg)
	{
	case WM_KEYDOWN:
		HandleKeyDownInput((int)wParam);
		break;
	case WM_KEYUP:
		HandleKeyUpInput((int)wParam);
		break;
	}

	return CallWindowProc(oWNDPROC, hWnd, msg, wParam, lParam);
}

void InitUI(LPDIRECT3DDEVICE9 pDevice)
{
	if (!initUI)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		UI::Init();

		if (!wndProcHooked)
		{
			oWNDPROC = (WNDPROC)SetWindowLongPtr(Game::Window, GWLP_WNDPROC, (LONG_PTR)WndProc);

			CreateDirectoryA("scripts\\CameraToolKit", NULL);
			CreateDirectoryA("scripts\\CameraToolKit\\Tracks", NULL);

			wndProcHooked = true;
		}

		ImGui_ImplWin32_Init(Game::Window);
		ImGui_ImplDX9_Init(pDevice);

		RECT rect;
		if (GetWindowRect(Game::Window, &rect))
		{
			WindowSize.x = rect.right - rect.left;
			WindowSize.y = rect.bottom - rect.top;
		}

		initUI = true;
	}
}

HRESULT __stdcall hookedEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	InitUI(pDevice);

	if (Globals::DrawUI)
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();

		UI::Draw();

		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}

	if (Globals::CameraOverride)
	{
		if (Globals::gTrackPlayer.IsPlaying())
		{
			Globals::gTrackPlayer.Update();
		}
		else if (!Globals::DrawUI)
		{
			Globals::gCameraManager.UpdateInputs();
		}

		Globals::gCameraManager.UpdateGamepad();
	}

	UpdateMouseInputs();
	UpdateGamepadInputs();

	if (Globals::DrawUI)
	{
		GetCursorPos(&LastMousePosUI);
	}

	return oEndScene(pDevice);
}

HRESULT __stdcall hookedReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	if (initUI)
	{
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		initUI = false;
		UI::Reset();
	}

	return oReset(pDevice, pPresentationParameters);
}

XMMATRIX* GetCarTransform()
{
	static XMMATRIX carMatrix;

	if (Game::State == 3)
	{
		carMatrix = XMMatrixIdentity();
	}

	if (Game::State == 6)
	{
		carMatrix = CarState::Player->Matrix;
		carMatrix.r[3] = CarState::Player->Position;
		carMatrix.r[3].m128_f32[3] = 1;
	}

	return &carMatrix;
}

typedef void(__thiscall* tSetCameraMatrix)(Game::Camera*, XMMATRIX*, float);
tSetCameraMatrix oSetCameraMatrix;
void __fastcall hSetCameraMatrix(Game::Camera* camera, int edx, XMMATRIX* matrix, float elapsed)
{
	if (camera == Game::PlayerCamera)
	{
		Globals::CamBackup = XMMatrixInverse(NULL, *matrix);
		if (Globals::CameraOverride)
		{
			if (Globals::gCameraMode == CameraMode::AttachToCar || Globals::gCameraMode == CameraMode::LookAtCar)
			{
				auto car = GetCarTransform();
				Globals::gCameraManager.SetCar(car);
				Globals::gTrackPlayer.SetCar(car);
			}

			if (Globals::gTrackPlayer.IsPlaying())
			{
				auto state = Globals::gTrackPlayer.GetState();
				*matrix = state.Matrix;
				camera->CurrentKey.horizontal_fov = state.Fov / 360.0 * 65535.0;
			}
			else
			{
				*matrix = Globals::gCameraManager.GetTransform();
				if (Globals::Settings.FovOverride)
				{
					float fov = Globals::gCameraManager.GetFov();
					camera->CurrentKey.horizontal_fov = fov / 360.0 * 65535.0;
				}
			}

			Globals::gCameraManager.ClearCar();
			Globals::gTrackPlayer.ClearCar();
		}
	}

	oSetCameraMatrix(camera, matrix, elapsed);
}

void Init()
{
	InitGlobals();

	LPDIRECT3DDEVICE9 pDevice = NULL;
	while (pDevice == NULL)
	{
		pDevice = *Game::Device;
		Sleep(1000);
	}

	auto vt = *(void***)pDevice;

	MH_Initialize();

	MH_CreateHook(vt[16], hookedReset, (LPVOID*)&oReset);
	MH_CreateHook(vt[42], hookedEndScene, (LPVOID*)&oEndScene);
	MH_CreateHook((LPVOID)0x00447280, hSetCameraMatrix, (LPVOID*)&oSetCameraMatrix);

	MH_EnableHook(MH_ALL_HOOKS);

	injector::WriteMemory<bool*>(0x005378C1, &Game::DrawFE);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x75BCC7) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
		{
			Globals::DllHandle = hModule;

			DisableThreadLibraryCalls(hModule);
			std::thread(Init).detach();
		}
		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.2 NTSC speed2.exe (4,57 MB (4.800.512 bytes)).", Globals::ToolName, MB_ICONERROR);
			return FALSE;
		}
	}

	return TRUE;
}