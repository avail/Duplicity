#include "chlamydia.h"
#include <codecvt>

static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> g_converter;

std::string ToNarrow(const std::wstring& wide)
{
	return g_converter.to_bytes(wide);
}

std::wstring ToWide(const std::string& narrow)
{
	return g_converter.from_bytes(narrow);
}

std::vector<double> NumbersFromString(const std::string& line)
{
	std::vector<double> ret;

	std::string number = "";
	for (int i = 0; i < line.length(); i++)
	{
		if (((int)line[i] >= 48 && (int)line[i] <= 57) || line[i] == '.' || line[i] == '-')
		{
			number += line[i];
		}
		else
		{
			if (number.length() > 0)
			{
				float nbr;
				sscanf(number.c_str(), "%f", &nbr);

				ret.push_back(nbr);
				number = "";
			}
		}
	}
	if (number.length() > 0)
	{
		float nbr;
		sscanf(number.c_str(), "%f", &nbr);

		ret.push_back(nbr);
		number = "";
	}
	return ret;
}

bool FileExists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}