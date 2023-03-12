//IOCP Session
//

#pragma once
#include <memory>

#include "SocketCommon.h"
#include "IOCPData.h"
#include "TCPSocket.h"
#include "SocketAddress.h"

namespace neo::network {
	class IOCPSession
	{
	public:
		IOCPSession();
		~IOCPSession();

	public:
		bool OnAccept( TCPSocket* socket, SocketAddress* addrInfo);
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

		TCPSocket* mTCPSocket;
		SocketAddress* mSocketAddress;


		std::atomic_bool mIsConneting;
		std::atomic_bool mIsSending;
	};
}