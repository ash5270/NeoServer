#include "IOCPSession.h"
#include"../system/InputMemoryStream.h"
#include"../system/OutputMemoryStream.h"
#include<memory.h>

neo::network::IOCPSession::IOCPSession() :mSendPacketQueue(16384),mMemoryPool(std::make_unique<system::MemoryPool>(Memory_Pool_Block_Size, Memory_Pool_Block_Count)),mOutputStream(mMemoryPool->Aollocate(), Memory_Pool_Block_Size)
{
	mSendData = std::make_shared<IOCPData>(IO_TYPE::IO_SEND);
	mRecvData = std::make_shared<IOCPData>(IO_TYPE::IO_READ);

	mRecvBuffer = std::make_unique<system::Buffer>(65536);
	//mSendData->SetBuffer(mMemoryPool->Aollocate(),mMemoryPool->GetBlockSize());
	mRecvData->SetBuffer(
		mRecvBuffer->GetDataPtr(),
		mRecvBuffer->GetCapacity()
	);
}

neo::network::IOCPSession::~IOCPSession()
{
	//smart pointer 삭제
	delete mSocketAddress;
}

bool neo::network::IOCPSession::OnAccept(TCPSocket* socket, SocketAddress* addrInfo)
{
	mTCPSocket = socket;
	mSocketAddress = addrInfo;
	return false;
}

void neo::network::IOCPSession::OnSend(size_t transferSize)
{

}
std::atomic<size_t> result;
void neo::network::IOCPSession::OnRecv(size_t transferSize)
{
	result.store(result.load() + transferSize);
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"Recv %d...\n", result.load());
	//wprintf_s(L"%lld\n",result.load());
	RecvReady();
}

void neo::network::IOCPSession::OnClose()
{

}

void neo::network::IOCPSession::AddRef()
{
	int count = Reference.load();
	Reference.exchange(count + 1);
}

void neo::network::IOCPSession::RemoveRef()
{
	int count = Reference.load();
	if (count <= 0)
	{
		//Session reference counting zero
		OnClose();
	}
	Reference.exchange(count - 1);
}

void neo::network::IOCPSession::RecvReady()
{
	mRecvData->GetWSABuf()->buf = mRecvData->GetBuffer() + mRecvData->GetSpan()->GetOffset();
	mRecvData->GetWSABuf()->len = mRecvData->GetSpan()->GetCapactiy() - mRecvData->GetSpan()->GetOffset();
	const auto result = mTCPSocket->WSARecv(
		mRecvData->GetWSABuf(),
		1,
		mRecvData->GetOverlapped(),
		NULL);
	if (result == SOCKET_ERROR && WSA_IO_PENDING != ::WSAGetLastError())
	{
		LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"recv ready error : %d\n", ::WSAGetLastError());
	}
}

void neo::network::IOCPSession::RecvReady(const size_t& offset, const size_t& haveRecvCount)
{
	auto leftBuf = mRecvData->GetBuffer() + offset;
	//LOG_PRINT(LOG_LEVEL::LOG_INFO, L"left buf size %d\n", Memory_Pool_Block_Size - haveRecvCount);
	//memmove
	memmove_s(mRecvData->GetBuffer(), haveRecvCount, mRecvData->GetBuffer() + offset, haveRecvCount);
	mRecvData->GetSpan()->SetOffset(haveRecvCount);

	WSABUF buf;
	buf.buf = mRecvData->GetBuffer() + mRecvData->GetSpan()->GetOffset();
	buf.len = mRecvData->GetSpan()->GetCapactiy() - mRecvData->GetSpan()->GetOffset();
	const auto result = mTCPSocket->WSARecv(
		&buf,
		1,
		mRecvData->GetOverlapped(),
		NULL);
	if (result == SOCKET_ERROR && WSA_IO_PENDING != ::WSAGetLastError())
	{
		LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"recv ready error : %d\n", ::WSAGetLastError());
	}
}

void neo::network::IOCPSession::SendIO()
{
	WSABUF buf;
	if (mSendPacketQueue.dequeue(buf))
	{
		//auto mem= mMemoryPool->Aollocate();
		//일단 더 보낼게 있는지 확인
		if (mSendPacketQueue.IsEmpty())
		{
			mSendData->SetBuffer(buf.buf, buf.len);
			int result = mTCPSocket->WSASend(mSendData->GetWSABuf(),
				1,
				mSendData->GetOverlapped());
			if (result == SOCKET_ERROR && (::WSAGetLastError() != ERROR_IO_PENDING))
			{
				LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"WSASend error : %d\n", ::WSAGetLastError());
				return;
			}
		}
		else {
		/*	auto mem = mMemoryPool->Aollocate();
			system::OutputMemoryStream output(mem, Memory_Pool_Block_Size);
			output.Write(buf.buf, buf.len);
			mMemoryPool->Free(buf.buf);
			WSABUF queBuf;*/

			bool isStop = false;
			//처음 꺼낸 버퍼 
			mOutputStream.Write(buf.buf, buf.len);
			mMemoryPool->Free(buf.buf);
			WSABUF queBuf;
			while (mSendPacketQueue.dequeue(queBuf))
			{
				if (!mOutputStream.Write(queBuf.buf, queBuf.len))
				{
					//mSendPacketQueue.enqueue(queBuf);
					isStop = true;
					break;
				}
				mMemoryPool->Free(queBuf.buf);
			}

			buf.buf = mOutputStream.GetStreamPtr();
			buf.len = mOutputStream.GetLength();

			mSendData->SetBuffer(buf.buf, buf.len);
			int result = mTCPSocket->WSASend(mSendData->GetWSABuf(),
				1,
				mSendData->GetOverlapped());
			if (result == SOCKET_ERROR && (::WSAGetLastError() != ERROR_IO_PENDING))
			{
				LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"WSASend error : %d\n", ::WSAGetLastError());
				return;
			}

			mOutputStream = system::OutputMemoryStream(mMemoryPool->Aollocate(), Memory_Pool_Block_Size);
			if (isStop)
			{
				mOutputStream.Write(queBuf.buf, queBuf.len);
			}
		}
	}
}
bool neo::network::IOCPSession::IsConnect()
{
	return mIsConneting.load();
}