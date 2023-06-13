//IOCP Session
//

#pragma once
#include <memory>

#include "SocketCommon.h"
#include "IOCPData.h"
#include "TCPSocket.h"
#include "SocketAddress.h"
//
#include "LockFreeQueue.h"
#include <ThreadSafeQueue.h>
#include <MPMCQueue.h>
#include "../system/MemoryPool.h"
#include"../system/Buffer.h"
#include"../system/OutputMemoryStream.h"

namespace neo::network {
	class IOCPSession
	{
	public:
		IOCPSession();
		virtual ~IOCPSession();
	public:
		virtual bool OnAccept(TCPSocket* socket, SocketAddress* addrInfo);
		virtual void OnSend(size_t transferSize);
		virtual void OnRecv(size_t transferSize);
		virtual void OnClose();

		void AddRef();
		void RemoveRef();
		//데이터를 receive 하기 위한 전 단계
		void RecvReady();
		//남아 있는 사이즈 만큼 앞으로 밀어준뒤 recv 받기 준비
		void RecvReady(const size_t & offset, const size_t& haveRecvCount);
		//queue에 담겨 있는 패킷 보내기
		void SendIO();

	    bool IsConnect();
	private:

	public:
		std::atomic_int32_t Reference;
	protected:
		std::shared_ptr<IOCPData> mRecvData;	
		std::shared_ptr<IOCPData> mSendData;
		//
		std::unique_ptr<system::Buffer> mRecvBuffer;
		//
		TCPSocket* mTCPSocket;
		SocketAddress* mSocketAddress;
		//체크
		std::atomic_bool mIsConneting;
		std::atomic_bool mIsSending;
		//메모리 풀
		std::unique_ptr<system::MemoryPool> mMemoryPool;
		//보낼 패킷 저장용
		util::system::MPMCQueue<WSABUF> mSendPacketQueue;

		//송신용 
		system::OutputMemoryStream mOutputStream;
	};
}