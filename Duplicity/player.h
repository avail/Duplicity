#pragma once

class Player
{
private:
	bool doubleTime = false;
	bool hasStarted;

	std::vector<HitObjectBase> objects;

	StopWatch watch;

public:
	inline Player()
	{
		this->hasStarted = false;

	}

	void SetDoubleTime(bool value);
	bool IsPlaying();
	void Reset();
	bool SetSong(std::vector<HitObjectBase> base);
	void StartFirstNote();
	void Update();
};