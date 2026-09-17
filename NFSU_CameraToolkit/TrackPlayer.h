#pragma once
#include <vector>
#include <DirectXMath.h>
#include <string>

#include "Utilities.h"

using namespace DirectX;
class TrackNode;

class TrackNodeModel
{
public:
	int nameIndex;
	XMFLOAT3 Position;
	XMVECTOR Rotation;
	float Fov;
	float Time;
	float Speed;
};

class TrackNode :public TrackNodeModel
{
private:
	char name[32];

public:
	float TimeStamp;

	TrackNode();
	TrackNode(int index);
	TrackNode(TrackNodeModel& model);

	char* GetName();

	TrackNodeModel ToModel();
};

struct TrackState
{
	XMFLOAT3 Position;
	XMVECTOR Rotation;
	XMMATRIX Matrix;
	float Fov;
};

class TrackPlayer;
class Track
{
private:
	std::vector<TrackNode*> nodes;
	float time;
	int currentNode;
	TrackPlayer* trackPlayer;
	CameraMode cameraMode;
	std::string name;
	int nodeCounter;
public:
	Track(TrackPlayer* trackPlayer, std::string& name);

	void AddNode();
	void AddNode(TrackNode* node);
	void PreviewNode(size_t index);
	void EditNode(size_t index);
	void RemoveNode(size_t index);
	void MoveNodeUp(size_t index);
	void MoveNodeDown(size_t index);
	bool Play();
	TrackNode Update();
	char* GetName();
	void SetName(char* name);
	std::vector<TrackNode*>& GetNodes();
	TrackNode* GetNode(size_t index);
	bool HasNodes();
	bool CameraModeMatches();
	CameraMode GetCameraMode();
	void SetCameraMode(CameraMode cameraMode);

	~Track();
private:
	void UpdateTimeStamps();
	bool IsIndexValid(size_t index);
	void InitNode(TrackNode* node);
};

class TrackPlayer
{
private:
	std::vector<Track*> tracks;
	bool isPlaying;
	TrackState currentState;
	XMMATRIX* CarTransform;
public:
	int TrackIndex;

	TrackPlayer();

	void AddTrack();
	void AddTrack(Track* track);
	void RemoveTrack();
	void AddNode();
	void Play();
	void Stop();
	void Update();
	bool IsPlaying();
	TrackState GetState();
	void ClearCar();
	void SetCar(XMMATRIX* carTransform);
	std::vector<Track*>& GetTracks();
	bool HasTracks();
	Track* GetCurrentTrack();
	void ResetIndex();
};