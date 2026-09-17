#pragma once
#include "UICommon.h"

namespace UI::About
{
	void Draw()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text(Globals::ToolName);

		ImGui::PushFont(io.Fonts->Fonts[4]);
		ImGui::Text("The tool to controll in-game camera and its various features.");
		ImGui::PopFont();

		ImGui::IncCursorPosY(20);
		ImGui::Text("CREDITS");
		ImGui::PushFont(io.Fonts->Fonts[4]);
		ImGui::BulletText("Dear ImGui by Omar Cornut (github.com/ocornut/imgui)");
		ImGui::BulletText("Cinematic Tools by Matti Hietanen (github.com/Hattiwatti/CinematicTools)");
		ImGui::PopFont();

		ImGui::IncCursorPosY(20);
		ImGui::Text("Please report any bugs to my Discord server or NFSMods page.");

		ImGui::IncCursorPosY(20);
		ImGui::Text("Made by ARCHIE");

		auto buttonSize = ButtonSize();
		ImGui::SetCursorPos1(ImVec2(10, 340));
		if (ImGui::Button("DISCORD", buttonSize))
		{
			ShellExecute(0, 0, L"https://discord.gg/DxC6Fyf", 0, 0, SW_SHOW);
		}
		ImGui::SameLine();
		if (ImGui::Button("NFS MODS", buttonSize))
		{
			ShellExecute(0, 0, L"https://nfsmods.xyz/mod/7664", 0, 0, SW_SHOW);
		}
	}
}