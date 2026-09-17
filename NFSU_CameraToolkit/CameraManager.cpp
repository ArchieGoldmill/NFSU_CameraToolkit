#include "CameraManager.h"
#include "Globals.h"
#include "Utilities.h"
#include "Game.h"

CameraManager::CameraManager()
{
	this->SetFov(45);
}

void CameraManager::UpdateInputs()
{
	this->UpdateKeys();
	this->UpdateRoll(Globals::IsKeyPressed(Globals::HK.RollCameraLeft), Globals::IsKeyPressed(Globals::HK.RollCameraRight));
	this->UpdateFOV(Globals::IsKeyPressed(Globals::HK.DecreaseFOV), Globals::IsKeyPressed(Globals::HK.IncreaseFOV));

	this->UpdateMatrix();
}

void CameraManager::UpdateKeys()
{
	float speed = Game::DeltaTime * Globals::Settings.MoveSpeed;

	float dx = 0;
	if (Globals::IsKeyPressed(Globals::HK.MoveCameraLeft))
	{
		dx = -speed;
	}
	if (Globals::IsKeyPressed(Globals::HK.MoveCameraRight))
	{
		dx = speed;
	}

	float dz = 0;
	if (Globals::IsKeyPressed(Globals::HK.MoveCameraForward))
	{
		dz = -speed;
	}
	if (Globals::IsKeyPressed(Globals::HK.MoveCameraBackward))
	{
		dz = speed;
	}

	this->ApplyMovement(dx, dz);

	if (Globals::IsKeyPressed(Globals::HK.MoveCameraUp))
	{
		this->Current.Position.z += speed;
	}
	if (Globals::IsKeyPressed(Globals::HK.MoveCameraDown))
	{
		this->Current.Position.z -= speed;
	}

	float mouseDx = 0;
	float mouseDy = 0;
	if (Globals::IsKeyPressed(Globals::HK.RotateCameraLeft))
	{
		mouseDx -= 4;
	}
	if (Globals::IsKeyPressed(Globals::HK.RotateCameraRight))
	{
		mouseDx += 4;
	}
	if (Globals::IsKeyPressed(Globals::HK.RotateCameraUp))
	{
		mouseDy -= 4;
	}
	if (Globals::IsKeyPressed(Globals::HK.RotateCameraDown))
	{
		mouseDy += 4;
	}

	if (mouseDx || mouseDy)
	{
		this->UpdateMouse(mouseDx, mouseDy, false);
	}
}

void CameraManager::ApplyMovement(float dx, float dz)
{
	if (dx || dz)
	{
		if (Globals::gCameraMode == CameraMode::LookAtCar)
		{
			this->Current.Position.x += dx;
			this->Current.Position.y += dz;
		}
		else
		{
			auto vPosition = XMLoadFloat3(&this->Current.Position);
			auto rotMatrix = XMMatrixRotationQuaternion(this->Current.Rotation);
			if (dz)
			{
				vPosition -= dz * rotMatrix.r[2];
			}

			if (dx)
			{
				vPosition += dx * rotMatrix.r[0];
			}

			XMStoreFloat3(&this->Current.Position, vPosition);
		}
	}
}

void CameraManager::UpdateMouse(float x, float y, bool updateMatrix)
{
	float mouseSpeed = 0.0001f * Globals::Settings.RotationSpeed;
	auto qPitch = XMQuaternionRotationAxis(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), y * mouseSpeed);
	auto qYaw = XMQuaternionRotationAxis(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), x * mouseSpeed);
	XMVECTOR qRotation = this->Current.Rotation;

	qRotation = XMQuaternionMultiply(qPitch, qRotation);
	qRotation = XMQuaternionMultiply(qRotation, qYaw);

	qRotation = XMQuaternionNormalize(qRotation);

	this->Current.Rotation = qRotation;

	if (updateMatrix)
	{
		this->UpdateMatrix();
	}
}

void CameraManager::UpdateGamepad()
{
	if (!g_Gamepad.IsConnected() || Globals::Settings.GamepadInput)
	{
		return;
	}

	// Update movement
	auto ls = g_Gamepad.GetLStick();
	float moveSpeed = Game::DeltaTime * Globals::Settings.MoveSpeed;
	this->ApplyMovement(ls.x * moveSpeed, ls.y * moveSpeed);

	// Update up down
	this->Current.Position.z += (g_Gamepad.GetRTrigger() - g_Gamepad.GetLTrigger()) * Game::DeltaTime * Globals::Settings.MoveSpeed * 20;

	// Update roation
	auto rs = g_Gamepad.GetRStick();
	this->UpdateMouse(-rs.x, Globals::Settings.GamepadInvert ? rs.y : -rs.y, false);

	this->UpdateRoll(g_Gamepad.IsPressed(XINPUT_GAMEPAD_LEFT_SHOULDER), g_Gamepad.IsPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER));

	this->UpdateFOV(g_Gamepad.IsPressed(XINPUT_GAMEPAD_DPAD_UP) || g_Gamepad.IsPressed(XINPUT_GAMEPAD_LEFT_THUMB),
		g_Gamepad.IsPressed(XINPUT_GAMEPAD_DPAD_DOWN) || g_Gamepad.IsPressed(XINPUT_GAMEPAD_RIGHT_THUMB));

	this->UpdateMatrix();
}

void CameraManager::UpdateMatrix()
{
	XMMATRIX rMatrix = XMMatrixRotationQuaternion(this->Current.Rotation);
	rMatrix.r[3] = XMLoadFloat3(&this->Current.Position);
	rMatrix.r[3].m128_f32[3] = 1;
	this->Transform = rMatrix;
}

void CameraManager::Reset()
{
	auto qPitch = XMQuaternionRotationAxis(XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f), DTR(90));
	auto qYaw = XMQuaternionRotationAxis(XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), DTR(90));

	this->Current.Rotation = XMQuaternionMultiply(qPitch, qYaw);

	if (Globals::gCameraMode == CameraMode::AttachToCar)
	{
		this->Current.Position = { -7, 0, 1 };
	}
	else
	{
		XMStoreFloat3(&this->Current.Position, Globals::CamBackup.r[3]);
	}

	this->UpdateMatrix();
}

void CameraManager::UpdateFOV(bool dec, bool inc)
{
	float fovSpeed = Game::DeltaTime * Globals::Settings.FovSpeed * 10.0f;
	float fov = this->Current.Fov;
	if (inc)
	{
		fov += fovSpeed;
	}
	if (dec)
	{
		fov -= fovSpeed;
	}

	this->SetFov(fov);
}

void CameraManager::UpdateRoll(bool left, bool right)
{
	float rollSpeed = Game::DeltaTime * Globals::Settings.RollSpeed / 10.0f;
	float deltaRoll = 0;
	if (left)
	{
		deltaRoll = rollSpeed;
	}
	if (right)
	{
		deltaRoll = -rollSpeed;
	}

	if (deltaRoll)
	{
		XMVECTOR rollQuaternion = XMQuaternionRotationAxis(XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), deltaRoll);
		auto qRotation = XMQuaternionMultiply(rollQuaternion, this->Current.Rotation);
		qRotation = XMQuaternionNormalize(qRotation);

		this->Current.Rotation = qRotation;
	}
}

void CameraManager::SetFov(float fov)
{
	Utilities::Clip(fov, 5, 120);
	this->Current.Fov = fov;
}

Camera CameraManager::GetCamera()
{
	return this->Current;
}

void CameraManager::SetCamera(XMFLOAT3 position, XMVECTOR rotation, float fov)
{
	this->Current.Position = position;
	this->Current.Rotation = rotation;
	this->Current.Fov = fov;
	this->UpdateMatrix();
}

XMMATRIX CameraManager::GetTransform()
{
	XMMATRIX rMatrix = this->Transform;
	if (this->CarTransform)
	{
		if (Globals::gCameraMode == CameraMode::AttachToCar)
		{
			rMatrix = XMMatrixMultiply(rMatrix, *this->CarTransform);
		}

		if (Globals::gCameraMode == CameraMode::LookAtCar)
		{
			auto carPosition = this->CarTransform->r[3];
			carPosition.m128_f32[2] += 0.75;
			auto up = XMVectorSet(0, 0, 1, 0);
			Game::eCreateLookAtMatrix(&rMatrix, &this->Current.Position, &carPosition, &up);
		}
	}

	if (Globals::gCameraMode == CameraMode::Free || Globals::gCameraMode == CameraMode::AttachToCar)
	{
		rMatrix = XMMatrixInverse(NULL, rMatrix);
	}

	return rMatrix;
}

float CameraManager::GetFov()
{
	return this->Current.Fov;
}

void CameraManager::SetCar(XMMATRIX* carTransform)
{
	this->CarTransform = carTransform;
}

void CameraManager::ClearCar()
{
	this->CarTransform = NULL;
}
