#include "IOCPSession.h"

neo::network::IOCPSession::IOCPSession()
{
	mSendData = std::make_shared<IOCPData>(IO_TYPE::IO_SEND);
	mRecvData = std::make_shared<IOCPData>(IO_TYPE::IO_READ);
}

neo::network::IOCPSession::~IOCPSession()
{
	//smart pointer 삭제
	mSendData.reset();
	mRecvData.reset();
}

bool neo::network::IOCPSession::OnAccept(const SOCKET& socket,const SOCKADDR_IN& addrInfo)
{
	int option = 1;
	mSocket = socket;
	mAddrInfo = addrInfo;

	const int result = setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&option), sizeof(option));
	if (result != 0)
		return false;

	mIsConneting.store(true);
	mIsSending.store(false);

	return true;
}

void neo::network::IOCPSession::OnSend(size_t transferSize)
{
	wprintf(L"OnSend %d\n", transferSize);
}

void neo::network::IOCPSession::OnRecv(size_t transferSize)
{
	wprintf(L"OnRecv % d\n", transferSize);
}

void neo::network::IOCPSession::OnClose()
{
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
	if(count<=0)
	{
		//Session reference counting zero

	}
	Reference.exchange(count + 1);
}

void neo::network::IOCPSession::RecvReady()
{
	WSABUF buf;
	buf.buf = mRecvData->GetBuffer();
	buf.len = 0;


	DWORD recvLen = 0;;
	DWORD flags = 0;

	auto result= WSARecv(mSocket, &buf,
		1, &recvLen, 
		&flags, mRecvData->GetOverlapped(), 
		NULL);

	if(result==SOCKET_ERROR)
	{
		wprintf_s(L"recv ready error : %d\n", WSAGetLastError());
	}
}
