#pragma once
#include "stopwatch.h"

class Player
{
private:
	bool doubleTime;
	bool hasStarted;
	int currentNote;
	int totalNotes;
	int noteDelay;

	std::vector<HitObjectBase> objects;

	StopWatch watch;

public:
	inline Player()
	{
		this->doubleTime = false;
		this->hasStarted = false;
		this->currentNote = 0;
		this->totalNotes = 0;
		this->noteDelay = 0;
	}

	void SetDoubleTime(bool value);
	bool IsPlaying();
	void Reset();
	bool SetSong(std::vector<HitObjectBase> base);
	void StartFirstNote();
	void Update();
	void PerformSingleClick(Vector2 position);
};