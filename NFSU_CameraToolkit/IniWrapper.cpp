#include "IniReader/IniReader.h"

CIniReader ini("NFSUCameraToolkit.ini");

namespace Ini
{
	int GetInt(const char* section, const char* name, int def = 0)
	{
		return ini.ReadInteger(section, name, def);
	}

	void SetInt(const char* section, const char* name, int value)
	{
		ini.WriteInteger(section, name, value);
	}

	float GetFloat(const char* section, const char* name, float def = 0)
	{
		return ini.ReadFloat(section, name, def);
	}

	void SetFloat(const char* section, const char* name, float value)
	{
		ini.WriteFloat(section, name, value);
	}

	bool GetBool(const char* section, const char* name, bool def = false)
	{
		return ini.ReadInteger(section, name, def ? 1 : 0) == 1;
	}

	void SetBool(const char* section, const char* name, bool value)
	{
		ini.WriteInteger(section, name, value ? 1 : 0);
	}
}