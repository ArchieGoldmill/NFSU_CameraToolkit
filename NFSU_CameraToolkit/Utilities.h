#pragma once
#include <Windows.h>
#include <vector>
#include <DirectXMath.h>
#include <string>
#include <algorithm>

using namespace DirectX;

const float M_PI = 3.14159265358979323846f;
#define DTR(a) a * (M_PI / 180) // Degrees to radians
#define RTD(a) a * (180 / M_PI) // Radians to degrees

enum class CameraMode
{
	None = -1,
	Free = 0,
	AttachToCar = 1,
	LookAtCar = 2
};

namespace Utilities
{
	bool GetResource(int ID, void*& pData, DWORD& size);
	float CatmullRomInterpolate(double y0, double y1, double y2, double y3, double mu);
	void UnprotectMemmory(void* mem, int size);
	void UnprotectAndNop(void* mem, int size);
	void Clip(float& val, float min, float max);
	void ClipMin(float& v, float min);

	uint64_t* GetPtr(uint64_t* offsets, int count);

	template<typename T>
	bool Contains(std::vector<T>& v, T x)
	{
		return std::find(v.begin(), v.end(), x) != v.end();
	}

	void GetFilesInFolder(std::vector<std::string>* list, const char* folder);

	XMFLOAT3 GetEuler(XMVECTOR quaternion);

	inline std::string ToLower(const std::string& str)
	{
		std::string result = str;
		std::transform(result.begin(), result.end(), result.begin(),
			[](unsigned char c) { return std::tolower(c); });
		return result;
	}

	inline bool ContainsIgnoreCase(const std::string str, const std::string sub)
	{
		std::string lowerStr = ToLower(str);
		std::string lowerSub = ToLower(sub);

		return lowerStr.find(lowerSub) != std::string::npos;
	}
}