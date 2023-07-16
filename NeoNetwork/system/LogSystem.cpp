#include "LogSystem.h"


neo::system::LogSystem::~LogSystem()
{

}

bool neo::system::LogSystem::InitSystem()
{
	mIsUpdating = false;
	::InitializeCriticalSection(&mCritical);
	this->OutPutLog(LogType::LOG_INFO, L"LogSystem Init...\n");
	//포트포워딩 안해서 서버 접속 불가
	//mHttpClient = std::make_unique<network::HttpClient>("192.168.123.104",3000);
	return false;
}

bool neo::system::LogSystem::StartSystem()
{
	mIsUpdating = true;
	this->OutPutLog(LogType::LOG_INFO, L"LogSystem Start...\n");
	return true;
}

void neo::system::LogSystem::StopSystem()
{
	mIsUpdating = false;

	::DeleteCriticalSection(&mCritical);
}

void neo::system::LogSystem::OutPutLog(LogType type, const std::wstring& log)
{
	EmplaceLogData(type, log);
}

void neo::system::LogSystem::AddLogData(LogData&& logData)
{
	::EnterCriticalSection(&mCritical);
	mLogDatas.push(std::move(logData));
	::LeaveCriticalSection(&mCritical);
}

void neo::system::LogSystem::EmplaceLogData(LogType type, const std::wstring& log)
{
	::EnterCriticalSection(&mCritical);
	mLogDatas.emplace(type, log);
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
		TypeSelectAndPrintLog(log);
		::LeaveCriticalSection(&mCritical);
	}
}

void neo::system::LogSystem::TypeSelectAndPrintLog(const LogData& logData)
{
	switch (logData.Type)
	{
	case LogType::LOG_INFO:
		wprintf_s(L"NEO || LOG_INFO ==> %s", logData.LogMessage.c_str());
		break;
	case LogType::LOG_DEBUG:
		wprintf_s(L"NEO || LOG_DEBUG ==> %s", logData.LogMessage.c_str());
		break;
	case LogType::LOG_ERROR:
		wprintf_s(L"NEO || LOG_ERROR ==> %s", logData.LogMessage.c_str());
		break;
	case LogType::LOG_WARNING:
		wprintf_s(L"NEO || LOG_WARNING ==> %s", logData.LogMessage.c_str());
		break;
	default:
		wprintf_s(L"NEO || Type Error ==> %s", logData.LogMessage.c_str());
		break;
	}
}
