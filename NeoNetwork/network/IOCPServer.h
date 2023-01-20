//IOCP 서버


#pragma once

#include "SocketCommon.h"
#include "IOCPSocket.h"
#include "IOCPData.h"

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

	protected:
		void OnAccept(const size_t& transferSize) override;

	private:
		//accept 준비//callback 함수로 사용하기 위해
		bool readyAccept();

	private:
		SOCKET mListenSocket;
		SOCKADDR_IN mServerAddr;

		bool mIsAccept;

		IOCPData mIOCPData;
	};
}