#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct Camera
{
	XMFLOAT3 Position;
	XMVECTOR Rotation;
	float Fov;
};

class CameraManager
{
private:
	Camera Current;
	XMMATRIX* CarTransform;
	XMMATRIX Transform;

public:
	CameraManager();

	void UpdateInputs();
	void UpdateMouse(float x, float y, bool updateMatrix);
	void UpdateGamepad();

	void Reset();

	XMMATRIX GetTransform();

	float GetFov();
	void SetFov(float fov);

	void SetCar(XMMATRIX* carTransform);
	void ClearCar();

	Camera GetCamera();
	void SetCamera(XMFLOAT3 position, XMVECTOR rotation, float fov);

private:
	void UpdateKeys();
	void UpdateMatrix();
	void UpdateFOV(bool dec, bool inc);
	void UpdateRoll(bool left, bool right);

	void ApplyMovement(float dx, float dz);
};