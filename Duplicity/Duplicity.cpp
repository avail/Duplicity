#include "chlamydia.h"

namespace Keys 
{
	static char PlaySongKey = 'C';
	static char DoubleTimeKey = 'V';
	static char ResetKey = 'B';
	static char QuitKey = 'N';
}

void PrintInfo()
{
	wprintf(L"Press %c to start the song on first note\n", Keys::PlaySongKey);
	wprintf(L"Press %c to enable the doubletime multiplier\n", Keys::DoubleTimeKey);
	wprintf(L"Press %c to reset to base state\n", Keys::ResetKey);
	wprintf(L"Press %c to exit\n", Keys::QuitKey);
	wprintf(L"\n");
}

HWND GetOsuHwnd()
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	DWORD result = NULL;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcessSnap) return(FALSE);

	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);
		return nullptr;
	}

	do
	{
		if (0 == wcscmp(L"osu!.exe", pe32.szExeFile))
		{
			result = pe32.th32ProcessID;
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	DWORD pid = result;

	std::pair<HWND, DWORD> params = { 0, pid };

	BOOL bResult = EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL
	{
		auto pParams = (std::pair<HWND, DWORD>*)(lParam);

		DWORD processId;
		if (GetWindowThreadProcessId(hwnd, &processId) && processId == pParams->second)
		{
			// Stop enumerating
			SetLastError(-1);
			pParams->first = hwnd;
			return FALSE;
		}

		// Continue enumerating
		return TRUE;
	}, (LPARAM)&params);

	if (!bResult && GetLastError() == -1 && params.first)
	{
		return params.first;
	}

	return nullptr;
}

HWND g_osuWindow;

int wmain(int argc, wchar_t* argv[])
{
	timeBeginPeriod(1);

	system("cls"); // clear console before beginning

	// we need an .osu file to run
	if (argc > 0)
	{

		std::wstring osuFile = argv[1];

		if (osuFile.substr(osuFile.find_last_of(L".") + 1) == L"osu")
		{
			// print some dumb info lol
			PrintInfo();

			// get osu! HWND
			g_osuWindow = GetOsuHwnd();

			if (g_osuWindow != nullptr)
			{
				wprintf(L"Found osu! window\n");
			}
			else
			{
				wprintf(L"how do you plan on running a cheat without a game\n");
#ifndef _DEBUG
				exit(0);
#endif
			}

			// load config
/*			Config config;
			std::string configFile = "duplicity.json";

			if (FileExists(configFile))
			{
				config.Load(configFile);
			}
			else
			{
				config.GenerateDefault(configFile);
				wprintf(L"No configuration file found - generated %s\n", configFile);
				wprintf(L"Modify to your liking then run duplicity again.\n");
				exit(0);
			}*/

			// parse beatmap
			wprintf(L"Trying to parse %s\n", osuFile.substr(osuFile.find_last_of(L"\\") + 1).c_str());

			beatmapParser parser;
			parser.ParseFile(ToNarrow(osuFile));
			wprintf(L"Finished parsing...\n");

			// vars

			bool allowDTToggle = false;
			bool doubleTime = false;
			bool songStarted = false;

			int delayCounter = GetTickCount();

			Player player;
			player.Reset();
			player.SetSong(parser.GetParsedObjects());

			// key-listener loop
			while (true)
			{
				if (player.IsPlaying())
				{
					player.Update();
				}

				if (!(GetTickCount() - delayCounter > 150))
				{
					continue;
				}

				// doubleTime
				if (GetAsyncKeyState(Keys::DoubleTimeKey) != 0)
				{
					if (doubleTime)
					{
						wprintf(L"doubleTime = false\n");
						doubleTime = false;
						player.SetDoubleTime(doubleTime);
					}
					else
					{
						wprintf(L"doubleTime = true\n");
						doubleTime = true;
						player.SetDoubleTime(doubleTime);
					}
				}

				if ((GetAsyncKeyState(Keys::PlaySongKey) != 0) && !songStarted)
				{
					wprintf(L"song started\n");
					player.StartFirstNote();
					songStarted = true;
				}

				if (GetAsyncKeyState(Keys::ResetKey) != 0 && songStarted)
				{
					player.Reset();
					songStarted = false;
					wprintf(L"song reset\n");
				}

				if (GetAsyncKeyState(Keys::QuitKey) != 0)
				{
					// clean exit?
					exit(0);
				}

				delayCounter = GetTickCount();
			}

			Sleep(50000); // deboog
		}
		else
		{
			wprintf(L"that's not an osu file\n");
			exit(0);
		}
	}
	else
	{
		wprintf(L"need an .osu file to run\n");
	}

    return 0;
}

