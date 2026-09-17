#pragma once
#include "UICommon.h"
#include "Globals.h"
#include "Utilities.h"
#include "Game.h"

namespace UI::Misc
{
	int forceLod = 0;
	void Draw()
	{
		if (ImGui::BeginTable("MiscTabTable", 3))
		{
			ImGui::TableNextRow();
			{
				ImGui::TableSetColumnIndex(0);
				{
					const char* forceLodComboItems[] = { "Default", "0", "1", "2", "3", "4" };
					if (DrawComboBox("Force LOD", "##ForceLOD", forceLodComboItems, 6, &forceLod))
					{
						Game::CarLod = forceLod - 1;
						Game::TireLOD = forceLod - 1;
					}
				}

				ImGui::TableSetColumnIndex(1);
				{
					DrawCheckBox("Draw HUD", &Game::DrawFE);
				}

				ImGui::TableSetColumnIndex(2);
				{
					ImGui::Text(" ");
					ImGui::IncCursorPosX(20);
					ImGui::Checkbox("Force rain", Game::ForceRain);
				}
			}

			ImGui::TableNextRow();
			{
				ImGui::TableSetColumnIndex(0);
				{
					ImGui::Text("");
					ImGui::SliderFloat("Steer angle", Game::FrontSteerAngle, -60.0, 60.0, "%.f");
				}

				ImGui::TableSetColumnIndex(1);
				{
					ImGui::Text("");
					ImGui::SliderFloat("Game speed", &Game::Speed, 0.01f, 1.0f, "%.3f");
				}
			}

			ImGui::EndTable();
		}
	}
}