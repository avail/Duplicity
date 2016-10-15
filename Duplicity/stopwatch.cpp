#include "chlamydia.h"

void StopWatch::Start()
{
	if (!started)
	{
		start = clock();
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
		return clock() - start;
	}

	return 0;
}

void StopWatch::Reset()
{
	if (started)
	{
		start = clock();
		started = false;
	}
}