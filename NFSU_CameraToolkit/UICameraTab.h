#pragma once
#include "UICommon.h"
#include "IniWrapper.h"
#include "Utilities.h"

namespace UI::Camera
{
	void DrawTab()
	{
		auto size = ButtonSize();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - size.x);
		if (ToggleButton(size, "ENABLE", "DISABLE", Globals::CameraOverride))
		{
			Globals::ToggleCamOverride();
		}

		ImGui::SameLine();
		if (ImGui::Button("RESET", size))
		{
			Globals::gCameraManager.Reset();
		}

		DrawLine(7);

		if (ImGui::BeginTable("CameraTabTable", 3))
		{
			ImGui::TableNextRow();
			{
				ImGui::TableSetColumnIndex(0);
				if (DrawFloatInput("Movement speed", "##CameraMovementSpeed", &Globals::Settings.MoveSpeed))
				{
					Utilities::ClipMin(Globals::Settings.MoveSpeed, 0);
					Ini::SetFloat("CAMERA_SETTINGS", "MoveSpeed", Globals::Settings.MoveSpeed);
				}

				ImGui::TableSetColumnIndex(1);

				if (DrawComboBox("Camera mode", "##CameraMode", CameraModeComboItems, 3, (int*)&Globals::gCameraMode))
				{
					Globals::gCameraManager.Reset();
				}

				ImGui::TableSetColumnIndex(2);
				if (DrawCheckBox("Use mouse to rotate", &Globals::Settings.UseMouseToRotate))
				{
					Globals::SetUseMouseToRotate();
				}
			}

			ImGui::TableNextRow();
			{
				ImGui::TableSetColumnIndex(0);
				if (DrawFloatInput("Rotation speed", "##CameraRotationSpeed", &Globals::Settings.RotationSpeed))
				{
					Ini::SetFloat("CAMERA_SETTINGS", "RotationSpeed", Globals::Settings.RotationSpeed);
				}

				ImGui::TableSetColumnIndex(1);
				float fov = Globals::gCameraManager.GetFov();
				if (DrawFloatInput("Field of view", "##CameraFov", &fov))
				{
					Globals::gCameraManager.SetFov(fov);
				}

				ImGui::TableSetColumnIndex(2);
				DrawCheckBox("FOV Override", &Globals::Settings.FovOverride);
			}

			ImGui::TableNextRow();
			{
				ImGui::TableSetColumnIndex(0);
				if (DrawFloatInput("Roll speed", "##CameraRollSpeed", &Globals::Settings.RollSpeed))
				{
					Ini::SetFloat("CAMERA_SETTINGS", "RollSpeed", Globals::Settings.RollSpeed);
				}

				if (g_Gamepad.IsConnected())
				{
					ImGui::TableSetColumnIndex(1);
					if (DrawCheckBox("Invert Gamepad", &Globals::Settings.GamepadInvert))
					{
						Ini::SetBool("GENERAL", "GamepadInvert", Globals::Settings.GamepadInvert);
					}
				}
			}

			ImGui::TableNextRow();
			{
				ImGui::TableSetColumnIndex(0);
				if (DrawFloatInput("FOV speed", "##CameraFovSpeed", &Globals::Settings.FovSpeed))
				{
					Ini::SetFloat("CAMERA_SETTINGS", "FovSpeed", Globals::Settings.FovSpeed);
				}

				if (g_Gamepad.IsConnected())
				{
					ImGui::TableSetColumnIndex(1);
					{
						DrawCheckBox("Gamepad Input", &Globals::Settings.GamepadInput);
					}
				}
			}

			ImGui::EndTable();
		}
	}
}