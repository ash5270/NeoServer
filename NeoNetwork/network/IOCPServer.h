//IOCP 서버
//IOCP Server
//서버 시작과 종료, 그리고 세션을 가지고 있음
#pragma once

#include <list>

#include "SocketCommon.h"
#include "IOCPSocket.h"
#include "IOCPData.h"
#include "IOCPSession.h"
#include"../packet/PacketID.h"
#include "LockFreeQueue.h"

namespace neo::network
{
	class IOCPServer : public IOCPSocket
	{
	public:	
		IOCPServer();
		~IOCPServer();

		//server 초기화
		bool InitializeServer(const int& port);
		//server 시작
		void StartServer();
		//server 중지
		void StopServer();
		//
		void UpdateServer();
		//CloseReady
		void CloseServer();

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
		std::shared_ptr<util::system::LockFreeQueue<Packet*>> mPacketQueue;

		std::unique_ptr<IOCPData> mIOCPData;
		//shared_ptr은 thread_safe하지 않아서 
		//그냥 원시 포인터 사용해야할듯
		std::list<IOCPSession*> mSessions;
	};
}