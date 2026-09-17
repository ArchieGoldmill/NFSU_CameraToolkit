#pragma once
#include <DirectXMath.h>
#include <stdio.h>

#include "ImGui/imgui.h"
#include "Globals.h"

using namespace DirectX;

#define VC(v) v / 255.0f // Vector color

namespace ImGui
{
	void RectFilled(const ImVec2& p_min, const ImVec2& p_max, ImU32 col)
	{
		auto drawList = ImGui::GetWindowDrawList();
		auto min = ImGui::GetCursorScreenPos();
		min.x += p_min.x;
		min.y += p_min.y;
		auto max = min;
		max.x += p_max.x;
		max.y += p_max.y;
		drawList->AddRectFilled(min, max, col);
	}

	void IncCursorPosY(float offset)
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + offset * Globals::UIScale);
	}

	void IncCursorPosX(float offset)
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset * Globals::UIScale);
	}

	void TextCentered(const char* text) {
		auto windowWidth = ImGui::GetWindowWidth();
		auto textWidth = ImGui::CalcTextSize(text).x;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text(text);
	}

	void SetCursorPosY1(float v)
	{
		SetCursorPosY(v * Globals::UIScale);
	}

	void SetCursorPosX1(float v)
	{
		SetCursorPosX(v * Globals::UIScale);
	}

	void SetCursorPos1(ImVec2 vec)
	{
		SetCursorPos({ vec.x * Globals::UIScale,vec.y * Globals::UIScale });
	}

	void RowSeparator()
	{
		ImGui::IncCursorPosY(20);
	}
}

namespace UI
{
	const auto ColorActive = ImVec4(VC(202), VC(241), VC(137), VC(255));
	const auto ColorHover = ImVec4(VC(194), VC(216), VC(164), VC(170));
	const auto ColorBlack = IM_COL32(0, 0, 0, 255);
	const auto ColorBlackV = ImVec4(VC(0), VC(0), VC(0), VC(255));
	const auto ColorWhite = IM_COL32(255, 255, 255, 255);
	const char* CameraModeComboItems[] = { "Free", "Attach to car", "Look at car" };
	const char* QualityScalableEnabledItems[] = { "Low", "Medium", "High", "Ultra", "Disabled" };

	ImVec2 ButtonSize()
	{
		return ImVec2(120 * Globals::UIScale, 30 * Globals::UIScale);
	}

	bool DrawButton(const char* label)
	{
		return ImGui::Button(label, ButtonSize());
	}

	bool ToggleButton(ImVec2 size, const char* offTxt, const char* onTxt, bool value)
	{
		if (value)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ColorWhite);
			ImGui::PushStyleColor(ImGuiCol_Text, ColorBlack);
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ColorBlack);
			ImGui::PushStyleColor(ImGuiCol_Text, ColorWhite);
		}

		bool res = ImGui::Button(value ? onTxt : offTxt, size);

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		return res;
	}

	bool DrawFloatInput(const char* text, const char* hash, float* value, float step = 1.0f, float width = 200)
	{
		if (text)
		{
			ImGui::Text(text);
		}

		ImGui::PushItemWidth(width * Globals::UIScale);
		bool result = ImGui::InputFloat(hash, value, step, step * 3, "%.3f");
		ImGui::PopItemWidth();

		return result;
	}

	bool DrawFloatInputHash(const char* text, float* value, float step = 1.0f, float width = 200)
	{
		ImGui::Text(text);

		char hash[128];
		sprintf_s(hash, "##%s", text);

		ImGui::PushItemWidth(width * Globals::UIScale);
		bool result = ImGui::InputFloat(hash, value, step, step * 3, "%.3f");
		ImGui::PopItemWidth();

		return result;
	}

	bool DrawComboBox(const char* text, const char* id, const char** items, int size, int* selected, float width = 180)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(230, 230, 230, 255));
		ImGui::Text(text);
		ImGui::PopStyleColor();
		ImGui::PushItemWidth(width);
		bool result = ImGui::Combo(id, selected, items, size);
		ImGui::PopItemWidth();

		return result;
	}

	bool DrawCheckBox(const char* text, bool* v)
	{
		ImGui::Text(" ");
		return ImGui::Checkbox(text, v);
	}

	bool DrawVector3(const char* text, XMFLOAT4* vec, bool header = false, float width = 150, float step = 0.1f)
	{
		bool updated = false;
		ImGui::TableNextRow();
		{
			char buf[128];
			ImGui::TableSetColumnIndex(0);
			{
				if (header)
				{
					ImGui::Text("");
				}

				ImGui::Text(text);
			}
			ImGui::TableSetColumnIndex(1);
			{
				sprintf_s(buf, "##%sX", text);
				updated |= DrawFloatInput(header ? "X" : NULL, buf, &vec->x, step, width);
			}
			ImGui::TableSetColumnIndex(2);
			{
				sprintf_s(buf, "##%sY", text);
				updated |= DrawFloatInput(header ? "Y" : NULL, buf, &vec->y, step, width);
			}
			ImGui::TableSetColumnIndex(3);
			{
				sprintf_s(buf, "##%sZ", text);
				updated |= DrawFloatInput(header ? "Z" : NULL, buf, &vec->z, step, width);
			}
		}

		return updated;
	}

	bool DrawVector4(const char* text, XMFLOAT4* vec, float width = 150, float step = 0.1f)
	{
		bool updated = false;
		ImGui::TableNextRow();
		{
			ImGui::TableSetColumnIndex(0);
			{
				ImGui::Text(text);
			}
		}

		ImGui::TableNextRow();
		{
			char buf[128];
			ImGui::TableSetColumnIndex(0);
			{
				sprintf_s(buf, "##%sX", text);
				updated |= DrawFloatInput(NULL, buf, &vec->x, step, width);
			}
			ImGui::TableSetColumnIndex(1);
			{
				sprintf_s(buf, "##%sY", text);
				updated |= DrawFloatInput(NULL, buf, &vec->y, step, width);
			}
			ImGui::TableSetColumnIndex(2);
			{
				sprintf_s(buf, "##%sZ", text);
				updated |= DrawFloatInput(NULL, buf, &vec->z, step, width);
			}
			ImGui::TableSetColumnIndex(3);
			{
				sprintf_s(buf, "##%sW", text);
				updated |= DrawFloatInput(NULL, buf, &vec->w, step, width);
			}
		}

		return updated;
	}

	void DrawVector4Header()
	{
		ImGui::TableNextRow();
		{
			char buf[128];
			ImGui::TableSetColumnIndex(0);
			{
				ImGui::Text("X");
			}
			ImGui::TableSetColumnIndex(1);
			{
				ImGui::Text("Y");
			}
			ImGui::TableSetColumnIndex(2);
			{
				ImGui::Text("Z");
			}
			ImGui::TableSetColumnIndex(3);
			{
				ImGui::Text("W");
			}
		}
	}

	void DrawRect(ImU32 color, float height)
	{
		float p = ImGui::GetStyle().WindowPadding.x * 2;
		ImGui::RectFilled({ 0, 0 }, { ImGui::GetWindowWidth() - p, height * Globals::UIScale }, color);
	}

	void DrawLine(int offset = 0)
	{
		ImGui::IncCursorPosY(offset);
		DrawRect(IM_COL32(200, 200, 200, 200), 1);
		ImGui::IncCursorPosY(offset);
	}

	void DrawModal(ImVec2 size, const char* name, const char* id, bool* show, void content())
	{
		if (!*show)
		{
			return;
		}

		size.x *= Globals::UIScale;
		size.y *= Globals::UIScale;
		ImGui::SetNextWindowSize(size, ImGuiCond_Always);

		ImGui::Begin(name, show, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);
		ImGui::BeginPopupContextItem(id);

		DrawRect(IM_COL32(255, 255, 255, 255), 24);

		ImGui::IncCursorPosY(2);
		ImGui::IncCursorPosX(2);
		ImGuiIO& io = ImGui::GetIO();
		ImGui::PushFont(io.Fonts->Fonts[4]);
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
		ImGui::Text(name);
		ImGui::PopStyleColor();
		ImGui::PopFont();

		ImGui::SameLine(size.x - 30 * Globals::UIScale);

		if (ImGui::Button("X", { 20 * Globals::UIScale,20 * Globals::UIScale }))
		{
			*show = false;
		}

		ImGui::IncCursorPosY(15);
		ImGui::PushFont(io.Fonts->Fonts[3]);
		content();
		ImGui::PopFont();

		ImGui::End();
	}

	void DrawError(char * text)
	{
		if (text)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::Text(text);
			ImGui::PopStyleColor();
		}
	}

	void DrawTooltip(const char* text, bool cond)
	{
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled | ImGuiHoveredFlags_DelayNormal) && cond)
		{
			ImGui::SetTooltip(text);
		}
	}
}