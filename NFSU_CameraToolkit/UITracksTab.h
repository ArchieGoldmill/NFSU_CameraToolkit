#pragma once
#include "UICommon.h"
#include "Globals.h"

namespace UI::Tracks
{
	bool ShowSaveModal = false;
	bool ShowLoadModal = false;
	bool ShowRemoveTrackModal = false;

	int SelectedNode = -1;
	char FileName[64] = { 0 };
	char* Error;

	bool SaveTrack()
	{
		char path[100];
		sprintf_s(path, "scripts\\CameraToolKit\\Tracks\\%s.bin", FileName);

		FILE* file = NULL;
		fopen_s(&file, path, "wb");

		if (file)
		{
			auto track = Globals::gTrackPlayer.GetCurrentTrack();
			auto nodes = track->GetNodes();
			auto count = (int)nodes.size();
			auto mode = (int)track->GetCameraMode();

			fwrite(&count, 4, 1, file);
			fwrite(&mode, 4, 1, file);
			for (int i = 0; i < count; i++)
			{
				auto node = nodes[i]->ToModel();
				fwrite(&node, sizeof(TrackNodeModel), 1, file);
			}

			fclose(file);
			return true;
		}

		return false;
	}

	void SaveModal()
	{
		ImGui::Text("File name:");
		ImGui::SameLine();
		ImGui::InputText("##FileName", FileName, sizeof(FileName), 0, 0, 0);
		ImGui::SameLine();
		ImGui::Text(".bin");

		ImGui::IncCursorPosY(15);
		if (DrawButton("Save"))
		{
			Error = NULL;

			if (strlen(FileName) == 0)
			{
				Error = (char*)"Enter the file name";
			}

			if (!Error)
			{
				if (SaveTrack())
				{
					ShowSaveModal = false;
					Globals::gTrackPlayer.GetCurrentTrack()->SetName(FileName);
				}
				else
				{
					Error = (char*)"Unable to save";
				}
			}
		}

		DrawError(Error);
	}

	int SelectedFile = -1;
	std::vector<std::string> Files;
	bool LoadTrack()
	{
		char path[100];
		sprintf_s(path, "scripts\\CameraToolKit\\Tracks\\%s", Files[SelectedFile].c_str());

		FILE* file = NULL;
		fopen_s(&file, path, "rb");

		if (file)
		{
			auto track = new Track(&Globals::gTrackPlayer, Files[SelectedFile]);
			Globals::gTrackPlayer.AddTrack(track);

			int count;
			CameraMode mode;
			fread_s(&count, 4, 4, 1, file);
			fread_s(&mode, 4, 4, 1, file);

			track->SetCameraMode(mode);
			for (int i = 0; i < count; i++)
			{
				TrackNodeModel model;
				fread_s(&model, sizeof(model), sizeof(model), 1, file);
				track->AddNode(new TrackNode(model));
			}

			fclose(file);
			return true;
		}

		return false;
	}

	void LoadModal()
	{
		if (ImGui::BeginListBox("##LoadTracksListBox", { 470 * Globals::UIScale, 195 * Globals::UIScale }))
		{
			for (int i = 0; i < Files.size(); i++)
			{
				if (ImGui::Selectable(Files[i].c_str(), SelectedFile == i))
				{
					SelectedFile = i;
				}
			}

			ImGui::EndListBox();
		}

		if (DrawButton("Load"))
		{
			Error = NULL;

			if (SelectedFile == -1)
			{
				Error = (char*)"Choose a file to load";
			}

			if (!Error)
			{
				if (LoadTrack())
				{
					ShowLoadModal = false;
				}
				else
				{
					Error = (char*)"Unable to load";
				}
			}
		}

		DrawError(Error);
	}

	void RemoveTrackModal()
	{
		ImGui::Text("Are you sure you want to remove the current track?");
		ImGui::IncCursorPosY(25);
		if (DrawButton("Yes"))
		{
			Globals::gTrackPlayer.RemoveTrack();

			ShowRemoveTrackModal = false;
		}

		ImGui::SameLine();
		ImGui::IncCursorPosX(100);
		if (DrawButton("Cancel"))
		{
			ShowRemoveTrackModal = false;
		}
	}

	void DrawSaveModal()
	{
		DrawModal({ 480, 150 }, "Save", "##Save", &ShowSaveModal, SaveModal);
	}

	void DrawLoadModal()
	{
		DrawModal({ 480, 310 }, "Load", "##Load", &ShowLoadModal, LoadModal);
	}

	void DrawRemoveTrackModal()
	{
		DrawModal({ 365, 170 }, "Remove track", "##RemoveTrack", &ShowRemoveTrackModal, RemoveTrackModal);
	}

	void DrawTracksComboBox()
	{
		auto tracks = Globals::gTrackPlayer.GetTracks();
		std::vector<char*> trackNames;
		for (auto track : tracks)
		{
			trackNames.push_back(track->GetName());
		}

		char** trackNamesPtr = {};
		if (trackNames.size())
		{
			trackNamesPtr = &trackNames[0];
		}

		ImGui::PushItemWidth(120 * Globals::UIScale);
		ImGui::Combo("##CameraTrack", &Globals::gTrackPlayer.TrackIndex, trackNamesPtr, (int)trackNames.size());
		ImGui::PopItemWidth();
	}

	size_t DrawNodesList()
	{
		ImGui::SetCursorPos1({ ImGui::GetStyle().WindowPadding.x, 180 });
		auto currentTrack = Globals::gTrackPlayer.GetCurrentTrack();
		size_t nodeCount = 0;

		if (ImGui::BeginListBox("##NodesListBox", { 200 * Globals::UIScale, 200 * Globals::UIScale }))
		{
			if (currentTrack)
			{
				auto nodes = currentTrack->GetNodes();
				nodeCount = nodes.size();
				for (int i = 0; i < nodeCount; i++)
				{
					if (ImGui::Selectable(nodes[i]->GetName(), SelectedNode == i))
					{
						SelectedNode = i;
					}
				}
			}

			ImGui::EndListBox();
		}

		return nodeCount;
	}

	void DrawTab()
	{
		auto buttonSize = ButtonSize();

		auto currentTrack = Globals::gTrackPlayer.GetCurrentTrack();
		if (currentTrack && !currentTrack->CameraModeMatches())
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
			ImGui::Text("Camera mode does not match! Intended mode:");

			ImGui::SameLine();
			ImGui::Text(CameraModeComboItems[(int)currentTrack->GetCameraMode()]);

			ImGui::PopStyleColor();
		}
		else
		{
			ImGui::Text("");
		}

		bool isPlaying = Globals::gTrackPlayer.IsPlaying();
		bool hasNodes = false;
		bool modeMatches = false;
		if (currentTrack)
		{
			hasNodes = currentTrack->HasNodes();
			modeMatches = currentTrack->CameraModeMatches();
		}

		if (ImGui::BeginTable("TracksTabTable", 5))
		{
			ImGui::TableNextRow();
			{
				ImGui::TableSetColumnIndex(0);
				ImGui::BeginDisabled(isPlaying || !hasNodes);
				if (ImGui::Button("PLAY", buttonSize))
				{
					Globals::gTrackPlayer.Play();
					SelectedNode = -1;
				}
				ImGui::EndDisabled();

				ImGui::TableSetColumnIndex(1);
				ImGui::BeginDisabled(!isPlaying);
				if (ImGui::Button("STOP", buttonSize))
				{
					Globals::gTrackPlayer.Stop();
				}
				ImGui::EndDisabled();

				ImGui::TableSetColumnIndex(2);
				ImGui::BeginDisabled(isPlaying || !Globals::CameraOverride || !modeMatches);
				if (ImGui::Button("ADD NODE", buttonSize))
				{
					Globals::gTrackPlayer.AddNode();
				}
				ImGui::EndDisabled();

				ImGui::TableSetColumnIndex(3);
				ImGui::BeginDisabled(isPlaying || SelectedNode == -1);
				if (ImGui::Button("REMOVE NODE", buttonSize))
				{
					currentTrack->RemoveNode(SelectedNode);
					SelectedNode = -1;
				}
				ImGui::EndDisabled();

				ImGui::TableSetColumnIndex(4);
				ImGui::BeginDisabled(isPlaying);
				if (ImGui::Button("ADD TRACK", buttonSize))
				{
					Globals::gTrackPlayer.AddTrack();
				}
				ImGui::EndDisabled();
			}

			ImGui::TableNextRow();
			{
				ImGui::TableSetColumnIndex(4);
				ImGui::BeginDisabled(isPlaying || !Globals::gTrackPlayer.HasTracks());
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(VC(220), VC(3), VC(1), VC(255)));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(VC(220), VC(3), VC(1), VC(170)));
				if (ImGui::Button("REMOVE TRACK", buttonSize))
				{
					ShowRemoveTrackModal = true;
				}
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::EndDisabled();
			}

			ImGui::TableNextRow();
			{
				ImGui::TableSetColumnIndex(4);
				ImGui::BeginDisabled(isPlaying || Globals::gTrackPlayer.TrackIndex < 0);
				if (ImGui::Button("SAVE", buttonSize))
				{
					strcpy_s(FileName, currentTrack->GetName());
					ShowSaveModal = true;
				}
				ImGui::EndDisabled();

				ImGui::BeginDisabled(isPlaying);
				if (ImGui::Button("LOAD", buttonSize))
				{
					Utilities::GetFilesInFolder(&Files, "scripts\\CameraToolKit\\Tracks");
					SelectedFile = -1;

					ShowLoadModal = true;
				}
				DrawTracksComboBox();
				ImGui::EndDisabled();
			}

			ImGui::EndTable();
		}

		size_t nodeCount = DrawNodesList();
		bool notSelected = SelectedNode < 0;

		ImGui::BeginDisabled(isPlaying || notSelected || SelectedNode == 0);
		ImGui::SetCursorPos1(ImVec2(220, 180));
		if (ImGui::Button("MOVE UP", buttonSize))
		{
			currentTrack->MoveNodeUp(SelectedNode);
			SelectedNode--;
		}
		ImGui::EndDisabled();

		ImGui::BeginDisabled(isPlaying || notSelected || SelectedNode == nodeCount - 1);
		ImGui::SetCursorPos1(ImVec2(220, 220));
		if (ImGui::Button("MOVE DOWN", buttonSize))
		{
			currentTrack->MoveNodeDown(SelectedNode);
			SelectedNode++;
		}
		ImGui::EndDisabled();

		bool notCamMode = notSelected || !Globals::CameraOverride;
		if (currentTrack)
		{
			notCamMode = notCamMode || !modeMatches;
		}

		ImGui::BeginDisabled(isPlaying || notCamMode);
		{
			ImGui::SetCursorPos1(ImVec2(220, 260));
			if (ImGui::Button("PREVIEW", buttonSize))
			{
				currentTrack->PreviewNode(SelectedNode);
			}

			ImGui::SetCursorPos1(ImVec2(220, 300));
			if (ImGui::Button("EDIT", buttonSize))
			{
				currentTrack->EditNode(SelectedNode);
			}
		}
		ImGui::EndDisabled();

		float nodeTime = 0;
		float* nodeTimePtr = &nodeTime;
		float* nodeSpeedPtr = &nodeTime;
		if (currentTrack)
		{
			auto node = currentTrack->GetNode(SelectedNode);
			if (node)
			{
				nodeTimePtr = &node->Time;
				nodeSpeedPtr = &node->Speed;
			}
		}

		ImGui::IncCursorPosY(50);
		if (ImGui::BeginTable("Node settings", 3))
		{
			ImGui::TableNextRow();
			{
				ImGui::BeginDisabled(isPlaying || notSelected);

				ImGui::TableSetColumnIndex(0);
				DrawFloatInput("Node time", "##nodeTime", nodeTimePtr, 0.5f);

				ImGui::TableSetColumnIndex(1);
				DrawFloatInput("Game speed", "##nodeSpeed", nodeSpeedPtr, 0.1f);

				ImGui::EndDisabled();
			}

			ImGui::EndTable();
		}
	}
}