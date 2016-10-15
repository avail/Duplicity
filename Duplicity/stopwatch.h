#pragma once

class StopWatch {
private:
	clock_t start;
	bool started;
	
public:
	inline StopWatch()
	{
		this->start = 0;
		this->started = false;
	}

	void Start();
	int Elapsed();
	void Stop();
	void Reset();
};