#include "IOCPSession.h"

#include"../packet/PacketAnalyzeAndCreate.h"
#include"../system/InputMemoryStream.h"
#include"../packet/Packet.h"
#include"../packet/PacketObject.h"
neo::network::IOCPSession::IOCPSession()
{
	mMemoryPool = std::make_unique<system::MemoryPool>(Memory_Pool_Block_Size, Memory_Pool_Block_Size);
	mSendData = std::make_shared<IOCPData>(IO_TYPE::IO_SEND);
	mRecvData = std::make_shared<IOCPData>(IO_TYPE::IO_READ);

	//mSendData->SetBuffer(mMemoryPool->Aollocate(),mMemoryPool->GetBlockSize());
	mRecvData->SetBuffer(mMemoryPool->Aollocate(), mMemoryPool->GetBlockSize());
}

neo::network::IOCPSession::~IOCPSession()
{
	//smart pointer 삭제
	delete mSocketAddress;

	mSendData.reset();
	mRecvData.reset();
}

bool neo::network::IOCPSession::OnAccept(TCPSocket* socket, SocketAddress* addrInfo,
	const std::shared_ptr<util::system::LockFreeQueue<packet::PacketObejct*>>& packetQueue)
{
	//mSocket = socket;
	mTCPSocket = socket;
	mSocketAddress = addrInfo;

	mIsConneting.store(true);
	mIsSending.store(false);

	//weak_ptr create
	mServerPacketQueue = packetQueue;

	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"OnAccept\n");

	return true;
}

void neo::network::IOCPSession::OnSend(size_t transferSize)
{
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"OnSend %d\n", transferSize);
	auto buf = mSendData->GetBuffer();
	mMemoryPool->Free(buf);
	
	WSABUF wsabuf;
	if (mSendPqcketQueue.Dequeue(wsabuf))
	{
		mSendData->SetBuffer(wsabuf);
	}
}
	
void neo::network::IOCPSession::OnRecv(size_t transferSize)
{
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"OnRecv %d \n", transferSize);
	InputMemoryStream inputStream(mRecvData->GetBuffer(), Memory_Pool_Block_Size);
	while (inputStream.GetLength() < transferSize)
	{
		auto newPacket = packet::PacketAnalyzeAndCreate::GetInstance().Analyzer(inputStream);
		packet::PacketObejct* packetObj = new packet::PacketObejct();
		packetObj->packet = newPacket;
		packetObj->session = this;

		//weak_ptr use
		auto queue = mServerPacketQueue.lock();
		queue->Enqueue(packetObj);
	}
	this->RecvReady();
}

void neo::network::IOCPSession::OnClose()
{
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"OnClosed\n");
	mIsConneting.exchange(false);

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

void neo::network::IOCPSession::SendPacket(Packet& packet)
{
	auto buffer = mMemoryPool->Aollocate();
	system::OutputMemoryStream output(buffer, mMemoryPool->GetBlockSize());
	packet.Serialize(output);
	WSABUF buf;
	buf.buf = buffer;
	buf.len = output.GetLength();
	mSendPqcketQueue.Enqueue(buf);
	if (!mSendPqcketQueue.Empty())
	{
		SendIO();
	}
}

void neo::network::IOCPSession::SendIO()
{
	WSABUF buf;
	if (mSendPqcketQueue.Dequeue(buf))
	{
		mSendData->SetBuffer(buf.buf,buf.len);
	    int result =mTCPSocket->WSASend(mSendData->GetWSABuf(),
			1,
			mSendData->GetOverlapped());
		if (result == SOCKET_ERROR && (::WSAGetLastError() != ERROR_IO_PENDING))
		{
			LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"WSASend error : %d\n",::WSAGetLastError());
			return;
		}
	}
}
