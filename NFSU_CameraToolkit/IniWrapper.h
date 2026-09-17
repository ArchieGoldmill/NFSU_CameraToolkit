#pragma once

namespace Ini
{
	int GetInt(const char* section, const char* name, int);

	void SetInt(const char* section, const char* name, int value);

	float GetFloat(const char* section, const char* name, float);

	void SetFloat(const char* section, const char* name, float value);

	bool GetBool(const char* section, const char* name, bool def);

	void SetBool(const char* section, const char* name, bool value);
}