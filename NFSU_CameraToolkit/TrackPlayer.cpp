#include "TrackPlayer.h"
#include "CameraManager.h"
#include "Utilities.h"
#include "Globals.h"
#include "Game.h"

Track::Track(TrackPlayer* trackPlayer, std::string& name)
{
	this->name = name;
	this->trackPlayer = trackPlayer;
	this->cameraMode = CameraMode::None;
	this->nodeCounter = 0;
}

void Track::AddNode()
{
	if (this->cameraMode == CameraMode::None)
	{
		this->cameraMode = Globals::gCameraMode;
	}

	if (this->CameraModeMatches())
	{
		auto node = new TrackNode(++this->nodeCounter);
		this->InitNode(node);
		node->Time = 5;
		node->Speed = 1;
		this->nodes.push_back(node);
	}
}

void Track::AddNode(TrackNode* node)
{
	this->nodes.push_back(node);
}

void Track::InitNode(TrackNode* node)
{
	auto camera = Globals::gCameraManager.GetCamera();
	node->Fov = camera.Fov;
	node->Position = camera.Position;
	node->Rotation = camera.Rotation;
}

void Track::PreviewNode(size_t index)
{
	if (this->IsIndexValid(index))
	{
		auto& node = this->nodes[index];
		Globals::gCameraManager.SetCamera(node->Position, node->Rotation, node->Fov);
	}
}

void Track::EditNode(size_t index)
{
	if (this->IsIndexValid(index))
	{
		this->InitNode(this->nodes[index]);
	}
}

void Track::RemoveNode(size_t index)
{
	if (this->IsIndexValid(index))
	{
		delete this->nodes[index];
		this->nodes.erase(this->nodes.begin() + index);
	}
}

void Track::MoveNodeUp(size_t index)
{
	if (index > 0 && this->nodes.size() > index)
	{
		auto item = this->nodes[index];
		this->nodes[index] = this->nodes[index - 1];
		this->nodes[index - 1] = item;
	}
}

void Track::MoveNodeDown(size_t index)
{
	if (index >= 0 && this->nodes.size() - 1 > index)
	{
		auto item = this->nodes[index];
		this->nodes[index] = this->nodes[index + 1];
		this->nodes[index + 1] = item;
	}
}

bool Track::Play()
{
	this->UpdateTimeStamps();
	this->time = 0;
	this->currentNode = 0;
	bool canPlay = this->nodes.size() > 1;
	if (canPlay)
	{
		Globals::CameraOverride = true;
		Globals::gCameraMode = this->cameraMode;
	}

	return canPlay;
}

TrackNode Track::Update()
{
	TrackNode resultNode;

	this->time += Game::DeltaTime;

	while (this->time > this->nodes[this->currentNode + 1]->TimeStamp)
	{
		if (this->currentNode + 1 < this->nodes.size() - 1)
		{
			this->currentNode++;
		}
		else
		{
			resultNode.Rotation = nodes[this->currentNode + 1]->Rotation;
			resultNode.Position = nodes[this->currentNode + 1]->Position;
			resultNode.Fov = nodes[this->currentNode + 1]->Fov;
			resultNode.Speed = nodes[this->currentNode + 1]->Speed;
			if (resultNode.Speed)
			{
				Game::Speed = 1;
			}

			this->trackPlayer->Stop();
			Globals::gCameraManager.SetCamera(resultNode.Position, resultNode.Rotation, resultNode.Fov);
			return resultNode;
		}
	}

	auto n1 = nodes.begin() + this->currentNode;
	auto n2 = n1 + 1;
	auto n0 = this->currentNode > 0 ? n1 - 1 : n1;
	auto n3 = this->currentNode + 1 < nodes.size() - 1 ? n2 + 1 : n2;

	float mu = (this->time - (*n1)->TimeStamp) / ((*n2)->TimeStamp - (*n1)->TimeStamp);

	auto qRot0 = (*n0)->Rotation;
	auto qRot1 = (*n1)->Rotation;
	auto qRot2 = (*n2)->Rotation;
	auto qRot3 = (*n3)->Rotation;

	auto vPos0 = XMLoadFloat3(&(*n0)->Position);
	auto vPos1 = XMLoadFloat3(&(*n1)->Position);
	auto vPos2 = XMLoadFloat3(&(*n2)->Position);
	auto vPos3 = XMLoadFloat3(&(*n3)->Position);

	resultNode.Fov = Utilities::CatmullRomInterpolate((*n0)->Fov,
		(*n1)->Fov,
		(*n2)->Fov,
		(*n3)->Fov, mu);

	resultNode.Speed = Utilities::CatmullRomInterpolate((*n0)->Speed,
		(*n1)->Speed,
		(*n2)->Speed,
		(*n3)->Speed, mu);

	XMVECTOR resultRot = XMVectorCatmullRom(qRot0, qRot1, qRot2, qRot3, mu);
	XMVECTOR resultPos = XMVectorCatmullRom(vPos0, vPos1, vPos2, vPos3, mu);

	resultNode.Rotation = XMQuaternionNormalize(resultRot);
	XMStoreFloat3(&resultNode.Position, resultPos);

	return resultNode;
}

char* Track::GetName()
{
	return (char*)this->name.c_str();
}

void Track::SetName(char* name)
{
	this->name = name;
}

std::vector<TrackNode*>& Track::GetNodes()
{
	return this->nodes;
}

TrackNode* Track::GetNode(size_t index)
{
	if (index >= 0 && index < this->nodes.size())
	{
		return this->nodes[index];
	}

	return NULL;
}

bool Track::HasNodes()
{
	return this->nodes.size() > 0;
}

bool Track::CameraModeMatches()
{
	if (this->cameraMode == CameraMode::None)
	{
		return true;
	}

	return this->cameraMode == Globals::gCameraMode;
}

CameraMode Track::GetCameraMode()
{
	return this->cameraMode;
}

void Track::SetCameraMode(CameraMode cameraMode)
{
	this->cameraMode = cameraMode;
}

Track::~Track()
{
	for (auto node : this->nodes)
	{
		delete node;
	}

	this->nodes.clear();
}

void Track::UpdateTimeStamps()
{
	if (this->nodes.size())
	{
		float timeStamp = 0;
		this->nodes[0]->TimeStamp = 0;
		for (int i = 1; i < this->nodes.size(); i++)
		{
			auto node = this->nodes[i];
			node->TimeStamp = node->Time + timeStamp;
			timeStamp = node->TimeStamp;
		}
	}
}

bool Track::IsIndexValid(size_t index)
{
	return index >= 0 && index < this->nodes.size();
}

TrackPlayer::TrackPlayer()
{
	this->isPlaying = false;
	this->CarTransform = NULL;
	this->currentState.Fov = 0;
	this->currentState.Matrix = XMMATRIX();

	this->AddTrack();
}

int TrackCounter = 0;
void TrackPlayer::AddTrack()
{
	auto name = "Track " + std::to_string(TrackCounter);
	this->tracks.push_back(new Track(this, name));
	TrackCounter++;
	this->ResetIndex();
}

void TrackPlayer::AddTrack(Track* track)
{
	this->tracks.push_back(track);
	this->ResetIndex();
}

void TrackPlayer::RemoveTrack()
{
	if (this->TrackIndex >= 0 && this->TrackIndex < this->tracks.size())
	{
		delete this->tracks[this->TrackIndex];
		this->tracks.erase(this->tracks.begin() + this->TrackIndex);
		this->ResetIndex();
	}
}

void TrackPlayer::ResetIndex()
{
	this->TrackIndex = (int)this->tracks.size() - 1;
}

void TrackPlayer::AddNode()
{
	if (Globals::CameraOverride && !this->IsPlaying())
	{
		auto currentTrack = this->GetCurrentTrack();
		if (!currentTrack)
		{
			this->AddTrack();
			currentTrack = this->GetCurrentTrack();
		}

		if (currentTrack)
		{
			currentTrack->AddNode();
		}
	}
}

void TrackPlayer::Play()
{
	if (this->HasTracks())
	{
		auto currentTrack = this->GetCurrentTrack();
		if (currentTrack->Play())
		{
			this->isPlaying = true;
		}
	}
}

void TrackPlayer::Stop()
{
	if (this->isPlaying)
	{
		Globals::gCameraManager.SetCamera(this->currentState.Position, this->currentState.Rotation, this->currentState.Fov);
	}

	this->isPlaying = false;
}

void TrackPlayer::Update()
{
	auto result = this->tracks[this->TrackIndex]->Update();
	this->currentState.Position = result.Position;
	this->currentState.Rotation = result.Rotation;

	this->currentState.Matrix = XMMatrixRotationQuaternion(result.Rotation);
	this->currentState.Matrix.r[3] = XMLoadFloat3(&result.Position);
	this->currentState.Matrix.r[3].m128_f32[3] = 1;
	this->currentState.Fov = result.Fov;
	Game::Speed = result.Speed;
}

bool TrackPlayer::IsPlaying()
{
	return this->isPlaying;
}

TrackState TrackPlayer::GetState()
{
	auto state = this->currentState;

	if (this->CarTransform)
	{
		if (Globals::gCameraMode == CameraMode::AttachToCar)
		{
			state.Matrix = XMMatrixMultiply(state.Matrix, *this->CarTransform);
		}

		if (Globals::gCameraMode == CameraMode::LookAtCar)
		{
			auto carPosition = this->CarTransform->r[3];
			carPosition.m128_f32[2] += 0.75;
			auto up = XMVectorSet(0, 0, 1, 0);
			Game::eCreateLookAtMatrix(&state.Matrix, &state.Matrix.r[3], &carPosition, &up);
		}
	}

	if (Globals::gCameraMode == CameraMode::Free || Globals::gCameraMode == CameraMode::AttachToCar)
	{
		state.Matrix = XMMatrixInverse(NULL, state.Matrix);
	}

	return state;
}

void TrackPlayer::ClearCar()
{
	this->CarTransform = NULL;
}

void TrackPlayer::SetCar(XMMATRIX* carTransform)
{
	this->CarTransform = carTransform;
}

std::vector<Track*>& TrackPlayer::GetTracks()
{
	return this->tracks;
}

bool TrackPlayer::HasTracks()
{
	return this->tracks.size() != 0;
}

Track* TrackPlayer::GetCurrentTrack()
{
	if (this->HasTracks())
	{
		return this->tracks[this->TrackIndex];
	}

	return NULL;
}

TrackNode::TrackNode()
{
	this->Fov = 0;
	this->Position = { 0,0,0 };
	this->Rotation = { 0,0,0,0 };
	this->Speed = 0;
	this->Time = 0;
	this->TimeStamp = 0;
	this->nameIndex = 0;
}

TrackNode::TrackNode(TrackNodeModel& model)
{
	this->nameIndex = model.nameIndex;
	this->Speed = model.Speed;
	this->Time = model.Time;
	this->Rotation = model.Rotation;
	this->Position = model.Position;
	this->Fov = model.Fov;
}

TrackNode::TrackNode(int nameIndex) : TrackNode()
{
	this->nameIndex = nameIndex;
}

char* TrackNode::GetName()
{
	sprintf_s(this->name, "%d) Time=%.2f, Speed=%.2f", this->nameIndex, this->Time, this->Speed);
	return this->name;
}

TrackNodeModel TrackNode::ToModel()
{
	TrackNodeModel model;

	model.Speed = this->Speed;
	model.Time = this->Time;
	model.Rotation = this->Rotation;
	model.Position = this->Position;
	model.Fov = this->Fov;
	model.nameIndex = this->nameIndex;

	return model;
}
