#include <Windows.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <vector>
#include "Globals.h"
#include "Utilities.h"

namespace fs = std::filesystem;

namespace Utilities
{
	bool GetResource(int ID, void*& pData, DWORD& size)
	{
		HRSRC rc = FindResource(Globals::DllHandle, MAKEINTRESOURCE(ID), RT_RCDATA);
		if (!rc)
		{
			return false;
		}

		HGLOBAL hglobal = LoadResource(Globals::DllHandle, rc);
		if (!hglobal)
		{
			return false;
		}

		pData = LockResource(hglobal);
		size = SizeofResource(Globals::DllHandle, rc);

		if (pData == nullptr || size == 0)
		{
			return false;
		}

		return true;
	}

	float CatmullRomInterpolate(double y0, double y1, double y2, double y3, double mu)
	{
		double mu2 = mu * mu;
		double a0 = -0.5 * y0 + 1.5 * y1 - 1.5 * y2 + 0.5 * y3;
		double a1 = y0 - 2.5 * y1 + 2 * y2 - 0.5 * y3;
		double a2 = -0.5 * y0 + 0.5 * y2;
		double a3 = y1;

		double res = a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3;
		return (float)res;
	}

	void ClipMin(float& v, float min)
	{
		if (v < min)
		{
			v = min;
		}
	}

	void Clip(float& v, float min, float max)
	{
		if (v > max)
		{
			v = max;
		}

		if (v < min)
		{
			v = min;
		}
	}

	void UnprotectMemmory(void* mem, int size)
	{
		DWORD old;
		VirtualProtect((LPVOID)mem, size, PAGE_EXECUTE_READWRITE, &old);
	}

	void UnprotectAndNop(void* mem, int size)
	{
		UnprotectMemmory(mem, size);
		memset(mem, 0x90, size);
	}

	uint64_t* GetPtr(uint64_t* offsets, int count)
	{
		auto ptr = (uint64_t*)offsets[0];
		for (int i = 1, size = count; i < size; i++)
		{
			if (ptr == NULL || *ptr == 0)
			{
				return 0;
			}

			ptr = (uint64_t*)(*ptr + offsets[i]);
		}

		return ptr;
	}

	void GetFilesInFolder(std::vector<std::string>* list, const char* folder)
	{
		list->clear();
		for (const auto& entry : fs::directory_iterator(folder))
		{
			list->push_back(entry.path().filename().generic_string());
		}
	}

	XMFLOAT3 GetEuler(XMVECTOR quaternion)
	{
		XMFLOAT4 q;
		XMStoreFloat4(&q, quaternion);

		double sqw = q.w * q.w;
		double sqx = q.x * q.x;
		double sqy = q.y * q.y;
		double sqz = q.z * q.z;

		double unit = sqx + sqy + sqz + sqw;
		double test = q.x * q.y + q.z * q.w;

		const float Epsilon = 1.401298E-45;
		const double eps = 0.5f - Epsilon;

		XMFLOAT3 result;

		if (test > eps * unit)
		{
			result.x = M_PI * 0.5f;
			result.y = 2.0f * (float)atan2(q.x, q.w);
			result.z = 0.0f;
		}
		else if (test < -eps * unit)
		{
			result.x = -M_PI * 0.5f;
			result.y = -2.0f * (float)atan2(q.x, q.w);
			result.z = 0.f;
		}
		else
		{
			result.x = (float)asin(2.f * test / unit);
			result.y = (float)atan2(2.f * q.y * q.w - 2.f * q.x * q.z, sqx - sqy - sqz + sqw);
			result.z = (float)atan2(2.f * q.x * q.w - 2.f * q.y * q.z, -sqx + sqy - sqz + sqw);
		}

		return result;
	}
}