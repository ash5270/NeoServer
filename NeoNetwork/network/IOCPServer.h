//IOCP 서버
//IOCP Server
//서버 시작과 종료, 그리고 세션을 가지고 있음
#pragma once

#include <list>
#include "SocketCommon.h"
#include "IOCPSocket.h"
#include "IOCPData.h"
#include "IOCPSession.h"

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
		virtual IOCPSession* OnAccept(TCPSocket* socket, SocketAddress* addrInfo);
		virtual void UpdateServer();
		//CloseReady
		void CloseServer();

	protected:
		void Accept(const size_t& transferSize) override;
		//accept 준비//callback 함수로 사용하기 위해
		bool readyAccept();
	protected:
		SOCKET mListenSocket;

		SOCKADDR_IN mServerAddr;
		TCPSocket mListen;
		TCPSocket* mClient;
		bool mIsAccept;
		
		std::unique_ptr<IOCPData> mIOCPData;
		std::unique_ptr<char> mIOCPBuffer;
	};
}