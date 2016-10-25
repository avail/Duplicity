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
	int stopHoldTime;
	bool isHolding;

	std::vector<HitObjectBase> objects;

	StopWatch watch;
	StopWatch moveWatch;

public:
	inline Player()
	{
		this->doubleTime = false;
		this->hasStarted = false;
		this->currentNote = 0;
		this->totalNotes = 0;
		this->noteDelay = 0;
		this->stopHoldTime = 0;
		this->isHolding = false;
	}

	void SetDoubleTime(bool value);
	bool IsPlaying();
	void Reset();
	bool SetSong(std::vector<HitObjectBase> base);
	void StartFirstNote();
	void Update();
	void CursorMovement(Vector2 position);
	void MaybePerformClickHold();
	void PerformSingleClick();
};