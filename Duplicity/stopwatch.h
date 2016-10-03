#pragma once

class StopWatch {
private:
	int start = 0;
	bool started = false;
	
public:
	inline StopWatch()
	{
		
	}

	void Initialize();
	void Start();
	int Elapsed();
	void Stop();
	void Reset();
};