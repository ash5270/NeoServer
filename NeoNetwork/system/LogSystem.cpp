#include "LogSystem.h"


neo::system::LogSystem::~LogSystem()
{
}

bool neo::system::LogSystem::InitSystem()
{
	return false;
}

bool neo::system::LogSystem::StartSystem()
{
	return false;
}

void neo::system::LogSystem::StopSystem()
{
}

void neo::system::LogSystem::AddLogData(LogData&& logData)
{
	std::lock_guard<std::mutex> lock(mLock);
	mLogDatas.push(std::move(logData));
}

void neo::system::LogSystem::Update()
{
}
