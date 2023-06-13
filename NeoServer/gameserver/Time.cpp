#include "Time.h"
neo::system::Time::Time() 
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&mPeriodFrequency);
	QueryPerformanceCounter((LARGE_INTEGER*)&mLastTime);
	mTimeSacle = 1.0f /(double)mPeriodFrequency.QuadPart;
}

neo::system::Time::~Time()
{
}

void neo::system::Time::Update()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&mCurTime);
	mDeltaTime = (double)(mCurTime.QuadPart - mLastTime.QuadPart)* mTimeSacle;
	mLastTime = mCurTime;
}

double neo::system::Time::GetDeltaTime() const
{
	return mDeltaTime;
}
