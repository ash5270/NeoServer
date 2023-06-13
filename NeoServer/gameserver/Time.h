#pragma once
#include <Windows.h>
namespace neo::system
{
	class Time
	{
	public:
		Time();
		~Time();

	public:
		void Update();
		double GetDeltaTime() const;
	private:
		double mDeltaTime;
		double mTimeSacle;
		
		LARGE_INTEGER mPeriodFrequency;
		LARGE_INTEGER mLastTime;
		LARGE_INTEGER mCurTime;
	};
}


