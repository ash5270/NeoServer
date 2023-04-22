//로그 출력을 위한 시스템
//싱글톤으로 작성
//또한 http를 통해서 웹서버로 log 전달
#pragma once
#include"../network/HttpClient.h"

#include <thread>
#include <queue>
#include <mutex>
#include <iostream>

#include "LogData.h"
#include "../Singleton.h"

namespace neo::system
{
	class LogSystem : public Singleton<LogSystem>
	{
	public:
		~LogSystem();
		bool InitSystem();
		bool StartSystem();
		void StopSystem();
		void OutPutLog(LogType type, const std::wstring& log);
		
	private:
		void AddLogData(LogData&& logData);
		void EmplaceLogData(LogType type, const std::wstring& log);
		//로그 데이터 업데이트
		//Queue에 들어있는 데이터를 꺼내서 출력
		void Update();
		//로그 타입별 지정후 출력
		void TypeSelectAndPrintLog(const LogData& logData);
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

