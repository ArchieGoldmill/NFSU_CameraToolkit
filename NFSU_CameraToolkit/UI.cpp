#include "UI.h"
#include "UICommon.h"
#include "Globals.h"
#include "Utilities.h"
#include "resource.h"

#include "UICameraTab.h"
#include "UITracksTab.h"
#include "UIAboutTab.h"
#include "UIMiscTab.h"

namespace UI
{
	enum class Tab
	{
		Camera,
		Tracks,
		Misc,
		About,
		_count
	};

	Tab CurrentTab = Tab::Camera;

	void InitFonts()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		io.IniFilename = "scripts\\CameraToolKit\\imgui.ini";

		void* pData;
		DWORD szData;
		ImFontConfig fontConfig;
		fontConfig.OversampleH = 8;
		fontConfig.OversampleV = 8;
		fontConfig.FontDataOwnedByAtlas = false;

		Utilities::GetResource(IDR_FONT_ROBOTO, pData, szData);
		io.Fonts->AddFontFromMemoryTTF(pData, szData, 10 * Globals::UIScale, &fontConfig);

		Utilities::GetResource(IDR_FONT_EUROSTYLE, pData, szData);
		io.Fonts->AddFontFromMemoryTTF(pData, szData, 30 * Globals::UIScale, &fontConfig);

		Utilities::GetResource(IDR_FONT_ROBOTO, pData, szData);
		io.Fonts->AddFontFromMemoryTTF(pData, szData, 22 * Globals::UIScale, &fontConfig);

		Utilities::GetResource(IDR_FONT_ROBOTO, pData, szData);
		io.Fonts->AddFontFromMemoryTTF(pData, szData, 18 * Globals::UIScale, &fontConfig);

		Utilities::GetResource(IDR_FONT_ROBOTO, pData, szData);
		io.Fonts->AddFontFromMemoryTTF(pData, szData, 16 * Globals::UIScale, &fontConfig);

		io.MouseDrawCursor = true;
	}

	void InitStyles()
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		style->Colors[ImGuiCol_ButtonActive] = ColorActive;
		style->Colors[ImGuiCol_ButtonHovered] = ColorHover;
		style->Colors[ImGuiCol_Button] = ColorBlackV;
		style->Colors[ImGuiCol_FrameBg] = ColorBlackV;
		style->Colors[ImGuiCol_FrameBgActive] = ColorActive;
		style->Colors[ImGuiCol_FrameBgHovered] = ColorHover;
		style->Colors[ImGuiCol_CheckMark] = ColorActive;
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(VC(252), VC(252), VC(252), VC(255));
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(VC(252), VC(252), VC(252), VC(255));
	}

	bool initUI = false;
	bool Init()
	{
		if (!initUI)
		{
			InitFonts();
			InitStyles();
			initUI = true;
		}

		return initUI;
	}

	void DrawHeader()
	{
		DrawRect(IM_COL32(255, 255, 255, 255), 50);

		ImGuiIO& io = ImGui::GetIO();
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
		ImGui::PushFont(io.Fonts->Fonts[1]);
		ImGui::IncCursorPosY(12);
		ImGui::TextCentered("CAMERA TOOLKIT");
		ImGui::PopFont();
		ImGui::PopStyleColor();
	}

	void TabButton(const char* on, Tab tab)
	{
		float p = ImGui::GetStyle().WindowPadding.x;
		float buttonW = (ImGui::GetWindowWidth() - p * 2) / ((int)Tab::_count);
		if (tab != Tab::Camera)
		{
			ImGui::SameLine((int)tab * buttonW + p);
		}

		if (ToggleButton(ImVec2(buttonW, 40 * Globals::UIScale), on, on, CurrentTab == tab))
		{
			CurrentTab = tab;
		}
	}

	void DrawTabButtons()
	{
		ImGui::SetCursorPosY1(58);

		ImGuiIO& io = ImGui::GetIO();
		ImGui::PushFont(io.Fonts->Fonts[2]);

		TabButton("CAMERA", Tab::Camera);

		TabButton("TRACKS", Tab::Tracks);

		TabButton("MISC", Tab::Misc);

		TabButton("ABOUT", Tab::About);

		ImGui::PopFont();
	}

	void DrawTab()
	{
		if (Globals::gTrackPlayer.IsPlaying())
		{
			CurrentTab = Tab::Tracks;
		}

		ImGuiIO& io = ImGui::GetIO();
		ImGui::PushFont(io.Fonts->Fonts[3]);
		ImGui::IncCursorPosY(10);
		switch (CurrentTab)
		{
		case Tab::Tracks:
			Tracks::DrawTab();
			break;
		case Tab::About:
			UI::About::Draw();
			break;
		case Tab::Misc:
			Misc::Draw();
			break;
		default:
			Camera::DrawTab();
			break;
		}

		ImGui::PopFont();
	}

	void RenderModals()
	{
		Tracks::DrawSaveModal();
		Tracks::DrawLoadModal();
		Tracks::DrawRemoveTrackModal();
	}

	bool IsModalShown()
	{
		return Tracks::ShowSaveModal || Tracks::ShowLoadModal || Tracks::ShowRemoveTrackModal;
	}

	void Draw()
	{
		ImGui::NewFrame();

		ImGuiIO& io = ImGui::GetIO();
		auto size = ImVec2(700, 380);
		if (CurrentTab == Tab::Tracks)
		{
			size.y = 450;
		}

		size.x *= Globals::UIScale;
		size.y *= Globals::UIScale;
		ImGui::SetNextWindowSize(size, ImGuiCond_Always);

		if (!ImGui::Begin(Globals::ToolName, NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize))
		{
			ImGui::End();
			return;
		}

		ImGui::BeginDisabled(IsModalShown());
		{
			DrawHeader();
			DrawTabButtons();
			DrawTab();
		}
		ImGui::EndDisabled();
		RenderModals();

		ImGui::End();
		ImGui::EndFrame();
	}

	void Reset()
	{
		initUI = false;
	}
}