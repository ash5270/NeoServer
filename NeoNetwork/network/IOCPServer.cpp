#include "IOCPServer.h"

#include <iostream>
#include <sysinfoapi.h>
#include <string>

#include "IOCPSession.h"
neo::network::IOCPServer::IOCPServer() :IOCPSocket()
{
	mIOCPData = std::make_unique<IOCPData>(IO_TYPE::IO_ACCEPT);
	mListenSocket = 0;
}

neo::network::IOCPServer::~IOCPServer()
{
	mIOCPData.release();
}

void neo::network::IOCPServer::StartServer()
{
	bool result = readyAccept();
	if (!result)
		return;
	wprintf_s(L"Server Start.......\n");
}

void neo::network::IOCPServer::StopServer()
{
	closesocket(mListenSocket);
	WSACleanup();
}

void neo::network::IOCPServer::UpdateServer()
{

}

void neo::network::IOCPServer::CloseServer()
{
	closesocket(mListenSocket);
	WSACleanup();
}

void neo::network::IOCPServer::OnAccept(const size_t& transferSize)
{
	//세션 생성
	SOCKADDR* localAddr;
	int localAddrSize = 0;
	//remote 연결되는 대상
	SOCKADDR* remoteAddr;
	int remoteAddrSize = 0;

	//get sockaddr info
	//Socket정보를 알아옴
	GetAcceptExSockaddrs((void*)mIOCPData->GetBuffer(),
		0,
		(sizeof(SOCKADDR_IN) + 16),
		(sizeof(SOCKADDR_IN) + 16),
		&localAddr,
		&localAddrSize,
		&remoteAddr,
		&remoteAddrSize);

	auto session = new IOCPSession();
	session->OnAccept(mIOCPData->GetSocket(), *(sockaddr_in*)remoteAddr);

	//register to session io completion port 
	mIOCPHandle = CreateIoCompletionPort(reinterpret_cast<HANDLE>(mIOCPData->GetSocket()),
		mIOCPHandle,
		reinterpret_cast<ULONG_PTR>(session),
		0);

	if (mIOCPHandle == NULL)
	{
		wprintf_s(L"session io completion port error\n");
	}

	session->AddRef();
	mSessions.push_back(session);

	session->RecvReady();
	readyAccept();
}

bool neo::network::IOCPServer::readyAccept()
{
	//clent socekt create
	const SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSock == SOCKET_ERROR)
	{
		//accept_socket create error 
		wprintf_s(L"accept socket create error\n");
		return false;	
	}

	//네이글 알고리즘 off 
	const int option = 1;
	int result = setsockopt(clientSock, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&option), sizeof(option));
	if (result != 0)
		return false;

	mIOCPData->SetSocket(clientSock);
	DWORD dwBytes;
	//비동기 accept 함수
	result = AcceptEx(mListenSocket, clientSock, mIOCPData->GetBuffer(),
		0,
		sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16,
		&dwBytes,
		mIOCPData->GetOverlapped());

	if (!result && (WSAGetLastError() != WSA_IO_PENDING))
	{
		//accept error
		wprintf_s(L"acceptex function error %d\n", WSAGetLastError());
		return false;
	}

	return true;
}

bool neo::network::IOCPServer::InitializeServer(const int& port)
{
	if (!WSAInit())
		wprintf_s(L"INIT error\n");

	//IOCP handle 생성
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	mIOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	int threadCount = systemInfo.dwNumberOfProcessors * 2;

	if (!CreateIOThread(threadCount))
	{
		wprintf_s(L"Io thread error\n");
		return false;
	}

	int result = 0;
	mListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//server host ip any 
	mServerAddr.sin_family = AF_INET;
	mServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	mServerAddr.sin_port = htons(port);

	bool on = true;
	if (setsockopt(mListenSocket, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (const char*)&on, sizeof(on)))
		return false;

	mIOCPHandle = CreateIoCompletionPort((HANDLE)mListenSocket,
		mIOCPHandle,
		(u_long)0,
		0);

	if (mIOCPHandle == NULL)
	{
		wprintf_s(L"acceptex io completion port  error\n");
	}

	//bind socket
	result = bind(mListenSocket, reinterpret_cast<SOCKADDR*>(&mServerAddr), sizeof(SOCKADDR_IN));
	if (result == SOCKET_ERROR)
	{
		//bind error
		wprintf_s(L"bind error %d\n", WSAGetLastError());
		CloseServer();
		return false;
	}

	DWORD len = 0;

	//listen backlog
	result = listen(mListenSocket, 100);
	if (result == SOCKET_ERROR)
	{
		//listen error
		wprintf_s(L"listen error\n");
		CloseServer();
		return false;
	}

	wprintf_s(L"Server Init........\n");
	return true;
}
