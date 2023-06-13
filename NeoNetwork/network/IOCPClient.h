//iocp client
//iocp 클라이언트
#pragma once

#include "SocketCommon.h"
#include "IOCPSocket.h"
#include "IOCPData.h"
#include "IOCPSession.h"

#include<memory>


namespace neo::network
{
	class IOCPClient : public IOCPSocket
	{
	public:
		IOCPClient(const std::string& ip, const int& port);
		virtual ~IOCPClient();

		//client 초기화
		bool InitializeClient( IOCPSession* session);
		//
	private:
		TCPSocket* mSocket;
		IOCPSession* mSession;
		SocketAddress mServerAddr;
	};

}

