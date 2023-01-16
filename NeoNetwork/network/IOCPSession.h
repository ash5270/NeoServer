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
	public:
		std::atomic_int32_t Reference;
	private:
		std::weak_ptr<IOCPData> mRecvData;
		std::weak_ptr<IOCPData> mSendData;
		SOCKET mSocket;
		std::atomic_bool mIsConneting;
		std::atomic_bool mIsSending;
	};
}