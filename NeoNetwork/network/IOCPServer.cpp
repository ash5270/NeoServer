﻿#include "IOCPServer.h"
#include <sysinfoapi.h>

neo::network::IOCPServer::IOCPServer() :IOCPSocket(), mIOCPData(IO_TYPE::IO_ACCEPT)
{
	mListenSocket = 0;
}

neo::network::IOCPServer::~IOCPServer()
{
}

void neo::network::IOCPServer::StartServer()
{
	
}

void neo::network::IOCPServer::StopServer()
{
}

void neo::network::IOCPServer::UpdateServer()
{
}

void neo::network::IOCPServer::OnAccept(const SOCKET& socket)
{
	//여기서 세션 생성해서 사용하면 된다.
	wprintf_s(L"Create Session\n");

	mIOCPHandle = CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket),
		mIOCPHandle,
		(u_long)0,
		0);

	if (mIOCPHandle == NULL)
	{
		wprintf_s(L"acceptex io completion port  error\n");
	}

	readyAccept();
}

bool neo::network::IOCPServer::readyAccept()
{
	SOCKET acceptSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (acceptSocket == SOCKET_ERROR)
	{
		//accept_socket create error
		wprintf_s(L"accept socket create error\n");
		return false;
	}

	mIOCPData.SetSocket(acceptSocket);
	DWORD dwBytes;
	if (AcceptEx(mListenSocket, acceptSocket, mIOCPData.GetBuffer(), 1024 - ((sizeof(sockaddr_in) + 16) * 2), sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16, &dwBytes, mIOCPData.GetOverlapped()))
	{
		//accept error
		wprintf_s(L"acceptex function error\n");
		return false;
	}

	return true;
}

bool neo::network::IOCPServer::InitializeServer(const int& port)
{
	if(!WSAInit())
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
	mListenSocket = socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);
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

	if(mIOCPHandle==NULL)
	{
		wprintf_s(L"acceptex io completion port  error\n");
	}

	result = bind(mListenSocket, reinterpret_cast<SOCKADDR*>( & mServerAddr), sizeof(SOCKADDR_IN));
	if(result==SOCKET_ERROR)
	{
		//bind error
		wprintf_s(L"bind error %d\n",WSAGetLastError());
		closesocket(mListenSocket);
		WSACleanup();
		return false;
	}

	DWORD len;

	result = listen(mListenSocket, 100);
	if(result==SOCKET_ERROR)
	{
		//listen error
		wprintf_s(L"listen error\n");
		closesocket(mListenSocket);
		WSACleanup();
		return false;
	}

	return readyAccept();
}
