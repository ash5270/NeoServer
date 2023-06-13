#include "IOCPServer.h"

#include <iostream>
#include <sysinfoapi.h>
#include <string>

#include "IOCPSession.h"
#include"../system/NeoLog.h"

neo::network::IOCPServer::IOCPServer() :IOCPSocket()
{
	mIOCPData = std::make_unique<IOCPData>(IO_TYPE::IO_ACCEPT);
	mIOCPBuffer = std::make_unique<char>(1024);
	mIOCPData->SetBuffer(mIOCPBuffer.get(), 1024);
	mListenSocket = 0;
}

neo::network::IOCPServer::~IOCPServer()
{
	
}

void neo::network::IOCPServer::StartServer()
{
	if(!system::LogSystem::GetInstance().StartSystem())
	{
		wprintf_s(L"LogSystem start Error\n");
	}
	//
	bool result = readyAccept();
	if (!result)
		return;
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"Server Start...\n");
}

void neo::network::IOCPServer::StopServer()
{
	closesocket(mListenSocket);
	WSACleanup();
}

neo::network::IOCPSession* neo::network::IOCPServer::OnAccept(TCPSocket* socket, SocketAddress* addrInfo)
{
	return nullptr;
}

void neo::network::IOCPServer::UpdateServer()
{

}

void neo::network::IOCPServer::CloseServer()
{
	closesocket(mListenSocket);
	WSACleanup();
}

void neo::network::IOCPServer::Accept(const size_t& transferSize)
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

	
	SocketAddress* clientAddr= new SocketAddress(*remoteAddr);
	auto session= OnAccept(mClient, clientAddr);

	//register to session io completion port 
	mIOCPHandle = CreateIoCompletionPort(reinterpret_cast<HANDLE>(mIOCPData->GetSocket()),
		mIOCPHandle,
		reinterpret_cast<ULONG_PTR>(session),
		0);

	if (mIOCPHandle == NULL)
	{ 
		LOG_PRINT(LOG_LEVEL::LOG_ERROR,L"session io completion port error\n");
	}

	session->AddRef();
	session->RecvReady();
	readyAccept();
}

bool neo::network::IOCPServer::readyAccept()
{
	////client socket create 
	mClient = mListen.AcceptEX(mIOCPData->GetBuffer(),mIOCPData->GetOverlapped());
	if (mClient == nullptr)
	{
		return false;
	}
	mIOCPData->SetSocket(mClient->GetSOCKET());
	return true;
}

bool neo::network::IOCPServer::InitializeServer(const int& port)
{
	//Log
	system::LogSystem::GetInstance().InitSystem();
	if (!WSAInit())
		LOG_PRINT(LOG_LEVEL::LOG_ERROR,L"INIT error\n");

	//IOCP handle 생성
	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);
	mIOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	int threadCount = systemInfo.dwNumberOfProcessors * 2;
	if (!CreateIOThread(threadCount))
	{
		LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"Io thread error\n");
		return false;
	}

	mListen.CreateSocket();
	mListen.SetNoDelay(true);

	SocketAddress address(INADDR_ANY, port);
	int result = mListen.Bind(address);
	if (result == SOCKET_ERROR)
	{
		LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"bind error %d\n", WSAGetLastError());
		CloseServer();
		return false;
	}

	mIOCPHandle = CreateIoCompletionPort((HANDLE)mListen.GetSOCKET(),
		mIOCPHandle,
		(u_long)0,
		0);

	if (mIOCPHandle == NULL)
	{
		LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"acceptex io completion port  error\n");
	}

	result = mListen.Listen();
	if (result == SOCKET_ERROR)
	{
		LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"bind error %d\n", WSAGetLastError());
		CloseServer();
		return false;
	}
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"Server Init...\n");
	return true;
}
