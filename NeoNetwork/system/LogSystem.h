//로그 출력을 위한 시스템

#pragma once
#include <thread>
#include <queue>
#include <mutex>

#include "LogData.h"
#include "../Singleton.h"
namespace neo::system
{
	class LogSystem :public Singleton<LogSystem>
	{
	private:
		~LogSystem();
	public:

		bool InitSystem();
		bool StartSystem();
		void StopSystem();
		void AddLogData(LogData&& logData);
		
	private:
		//로그 데이터 업데이트
		//Queue에 들어있는 데이터를 꺼내서 출력
		void Update();

	private:
		std::thread mLogThread;
		std::queue<LogData> mLogDatas;
		std::mutex mLock;
	};
}

