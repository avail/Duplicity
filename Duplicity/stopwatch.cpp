#include "chlamydia.h"

void StopWatch::Start()
{
	if (!started)
	{
		start = GetTickCount();
		started = true;
	}
}

void StopWatch::Stop()
{
	if (started)
	{
		started = false;
	}
	else
	{
		wprintf(L"stopwatch not started - what\n");
	}
}

int StopWatch::Elapsed()
{
	if (started)
	{
		return GetTickCount() - start;
	}
}

void StopWatch::Reset()
{
	if (started)
	{
		start = GetTickCount();
		started = false;
	}
}