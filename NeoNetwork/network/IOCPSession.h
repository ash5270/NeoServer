//IOCP Session
//

#pragma once
#include <memory>

#include "SocketCommon.h"
#include "IOCPData.h"

namespace neo::network {
	class IOCPSession
	{
	public:
		IOCPSession();
		~IOCPSession();

	public:
		bool OnAccept(const SOCKET& socket,const SOCKADDR_IN& addrInfo);
		void OnSend(size_t transferSize);
		void OnRecv(size_t transferSize);

		void OnClose();
		void AddRef();
		void RemoveRef();
		//데이터를 receive 하기 위한 전 단계
		void RecvReady();

	private:
		

	public:
		std::atomic_int32_t Reference;
	private:
		std::shared_ptr<IOCPData> mRecvData;
		std::shared_ptr<IOCPData> mSendData;

		SOCKET mSocket;
		SOCKADDR_IN mAddrInfo;

		std::atomic_bool mIsConneting;
		std::atomic_bool mIsSending;
	};
}