//IOCP 서버
//IOCP Server
//서버 시작과 종료, 그리고 세션을 가지고 있음
#pragma once

#include <list>
#include<functional>

#include "SocketCommon.h"
#include "IOCPSocket.h"
#include "IOCPData.h"
#include "IOCPSession.h"
#include"../packet/PacketID.h"
#include"../system/LogicThread.h"
#include"../system/PacketProcessThread.h"

namespace neo::network
{
	class IOCPServer : public IOCPSocket
	{
	public:
		IOCPServer();
		virtual ~IOCPServer();

		//server 초기화
		bool InitializeServer(const int& port);
		//server 시작
		void StartServer();
		//server 중지
		void StopServer();
		//
		virtual void UpdateServer();
		//CloseReady
		void CloseServer();

		std::vector<system::LogicThread*>& GetLogicThread()
		{
			return mLogicThreads;
		}

		PacketProcessThread* GetNonLogicThread()
		{
			return mNonLogicThread.get();
		}

	protected:
		void OnAccept(const size_t& transferSize) override;

	private:
		//accept 준비//callback 함수로 사용하기 위해
		bool readyAccept();

	private:
		SOCKET mListenSocket;

		SOCKADDR_IN mServerAddr;
		TCPSocket mListen;
		TCPSocket* mClient;
		bool mIsAccept;

		//packet queue
		const int mLogicThreadCount = 2;
		std::vector<system::LogicThread*> mLogicThreads;
		unique_ptr<system::PacketProcessThread> mNonLogicThread;
		std::unique_ptr<IOCPData> mIOCPData;
		std::unique_ptr<char> mIOCPBuffer;
		//shared_ptr은 thread_safe하지 않아서 
		//그냥 원시 포인터 사용해야할듯
		std::list<IOCPSession*> mSessions;

	public:

	};
}