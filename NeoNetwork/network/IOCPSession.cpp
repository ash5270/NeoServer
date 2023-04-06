#include "IOCPSession.h"

#include"../packet/PacketAnalyzeAndCreate.h"
#include"../system/InputMemoryStream.h"
#include"../packet/Packet.h"

neo::network::IOCPSession::IOCPSession()
{
	mMemoryPool = std::make_unique<system::MemoryPool>(Memory_Pool_Block_Size, Memory_Pool_Block_Size);
	mSendData = std::make_shared<IOCPData>(IO_TYPE::IO_SEND);
	mRecvData = std::make_shared<IOCPData>(IO_TYPE::IO_READ);

	mSendData->SetBuffer()

}

neo::network::IOCPSession::~IOCPSession()
{
	//smart pointer 삭제
	delete mSocketAddress;
	mSendData.reset();
	mRecvData.reset();
}

bool neo::network::IOCPSession::OnAccept(TCPSocket* socket, SocketAddress* addrInfo,
	const std::shared_ptr<util::system::LockFreeQueue<Packet*>>& packetQueue)
{
	//mSocket = socket;
	mTCPSocket = socket;
	mSocketAddress = addrInfo;

	mIsConneting.store(true);
	mIsSending.store(false);

	//weak_ptr create
	mServerPacketQueue = packetQueue;

	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"OnAccept\n");

	P_S_RES_LOGIN packet;
	packet.status_code = 1400;
	system::OutputMemoryStream output(*mSendData->GetBuffer());
	packet.Serialize(output);
	mTCPSocket->WSASend(mSendData->GetWSABuf(), 1, mSendData->GetOverlapped());
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
	InputMemoryStream inputStream(*mRecvData->GetBuffer());
	packet::PacketAnalyzeAndCreate::GetInstance().Analyzer(inputStream);

	//WSABUF buf;
	//buf.buf = mRecvData->GetBuffer()->GetDataPtr();
	////buf.len = mRecvData->GetBuffer()->GetCapacity();
	//buf.len = transferSize;

	//DWORD recvLen = 0;;
	//DWORD flags = 0;

	//const auto result = WSASend(mSocket, &buf,
	//	1, &recvLen,
	//	flags, mSendData->GetOverlapped(),
	//	NULL); 

	//recevice 대기
	this->RecvReady();
}

void neo::network::IOCPSession::OnClose()
{
	wprintf(L"OnClose\n");
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
	DWORD recvLen = 0;;
	DWORD flags = 0;

	const auto result = mTCPSocket->WSARecv(mRecvData->GetWSABuf(), 1, mRecvData->GetOverlapped(), NULL);
	if (result == SOCKET_ERROR && WSA_IO_PENDING != WSAGetLastError())
	{
		wprintf_s(L"recv ready error : %d\n", GetLastError());
	}
}

void neo::network::IOCPSession::SendPacket(Packet& packet)
{
	auto buffer = mMemoryPool->Aollocate();
	system::OutputMemoryStream output(buffer, 1024);
	packet.Serialize(output);
	WSABUF buf;
	buf.buf = buffer;
	buf.len = output.GetLength();
	mSendPqcketQueue.Enqueue(buf);
}
