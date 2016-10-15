#pragma once

#pragma comment(lib, "Kernel32.lib")

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <string>
#include <tlhelp32.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <time.h>

#include "beatmapParser.h"
#include "bezier.h"
#include "player.h"
#include "stopwatch.h"
//#include "config.h"

std::wstring ToWide(const std::string& narrow);
std::string ToNarrow(const std::wstring& wide); 
std::vector<double> NumbersFromString(const std::string& line);
bool FileExists(const std::string& name);
Vector2 ConvertToScreen(Vector2 point);
void Convert(Vector2 &point, int resX, int resY);
void MoveCursor(Vector2 position);
void Click();
void Release();

extern HWND g_osuWindow;