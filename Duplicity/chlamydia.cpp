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

void Convert(Vector2 &point, int resX, int resY)
{
	point.x = (1.6f * point.x) + 106.0f;
	point.y = (1.4f * point.y) + 106.0f;

	point.x = (float)point.x * (float)resX / 1024.0f;
	point.y = (float)point.y * (float)resY / 768.0f;
}

void PressKey(int key)
{
	INPUT inputData;
	inputData.type = INPUT_KEYBOARD;
	inputData.ki.wScan = 0;
	inputData.ki.time = 0;
	inputData.ki.dwExtraInfo = 0;
	inputData.ki.wVk = key;
	inputData.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;

	if (SendInput(1, &inputData, sizeof(INPUT)) == 0)
	{
		wprintf(L"errorino %d\n", GetLastError());
	}
}

void ReleaseKey(int key)
{
	INPUT inputData;
	inputData.type = INPUT_KEYBOARD;
	inputData.ki.wScan = 0;
	inputData.ki.time = 0;
	inputData.ki.dwExtraInfo = 0;
	inputData.ki.wVk = key;
	inputData.ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(1, &inputData, sizeof(INPUT));
}

void Click()
{
	// TODO: random logic to alternate all 4 keys
	PressKey('M');
}

void Release()
{
	ReleaseKey('M');
}