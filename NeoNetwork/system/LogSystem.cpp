#include "LogSystem.h"


neo::system::LogSystem::~LogSystem()
{

}

bool neo::system::LogSystem::InitSystem()
{
	mIsUpdating = false;
	::InitializeCriticalSection(&mCritical);

	//포트포워딩 안해서 서버 접속 불가
	//mHttpClient = std::make_unique<network::HttpClient>("192.168.123.104",3000);
	return false;
}

bool neo::system::LogSystem::StartSystem()
{
	mIsUpdating = true;
	mLogThread = std::thread(&LogSystem::Update, this);


	return false;
}

void neo::system::LogSystem::StopSystem()
{
	mIsUpdating = false;

	if (mLogThread.joinable())
		mLogThread.join();

	::DeleteCriticalSection(&mCritical);
}

void neo::system::LogSystem::AddLogData(LogData&& logData)
{
	::EnterCriticalSection(&mCritical);
	mLogDatas.push(std::move(logData));
	::LeaveCriticalSection(&mCritical);
}

void neo::system::LogSystem::Update()
{
	while (mIsUpdating)
	{
		//비어있을때 의미없는 사용을 막기 위해 
		//context switch 해줌
		if (mLogDatas.empty()) 
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
			continue;
		}

		::EnterCriticalSection(&mCritical);
		const LogData  log = mLogDatas.front();
		mLogDatas.pop();
		std::wcout << log.LogMessage << "\n";

		::LeaveCriticalSection(&mCritical);
	}
}
