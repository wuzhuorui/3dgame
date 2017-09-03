#pragma once

class GameTimer
{
public:
	GameTimer();
	float DeltaTime()const; // in seconds

	void Reset();
	void Stop(); 
	void Start(); 
	float TotalTime()const;

	void Tick(); // Call every frame.
private:

	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;
	bool mStopped;
};