//로그 출력을 위한 시스템
//싱글톤으로 작성
//또한 http를 통해서 웹서버로 log 전달
#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <Windows.h>
#include <iostream>

#include "LogData.h"
#include "../Singleton.h"

#include "../network/HttpClient.h"
namespace neo::system
{
	class LogSystem : public Singleton<LogSystem>
	{
	public:
		~LogSystem();
		bool InitSystem();
		bool StartSystem();
		void StopSystem();
		void AddLogData(LogData&& logData);
		
	private:
		//로그 데이터 업데이트
		//Queue에 들어있는 데이터를 꺼내서 출력
		void Update();
		//logdata log내용 수정	
	private:
		//update 유무
		bool mIsUpdating;
		//LogThread
		std::thread mLogThread;
		std::queue<LogData> mLogDatas;
		
		//http client
		//std::unique_ptr<network::HttpClient> mHttpClient;

		//뮤테스 대신 크리티컬 섹션 사용
		CRITICAL_SECTION mCritical;
	};
}

