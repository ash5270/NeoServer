﻿#include "IOCPServer.h"

#include <iostream>
#include <sysinfoapi.h>
#include <string>


#include "IOCPSession.h"
#include"../system/NeoLog.h"

neo::network::IOCPServer::IOCPServer() :IOCPSocket()
{
	mIOCPData = std::make_unique<IOCPData>(IO_TYPE::IO_ACCEPT);
	mIOCPBuffer = std::make_unique<char>(1024);
	mNonLogicThread = std::make_unique<system::PacketProcessThread>();
	mIOCPData->SetBuffer(mIOCPBuffer.get(), 1024);
	mListenSocket = 0;
}

neo::network::IOCPServer::~IOCPServer()
{
	mIOCPData.release();
}

void neo::network::IOCPServer::StartServer()
{
	if(!system::LogSystem::GetInstance().StartSystem())
	{
		wprintf_s(L"LogSystem start Error\n");
	}
	//packet process thread
	for (int i = 0; i < mLogicThreadCount; i++)
		mLogicThreads[i]->Start();
	mNonLogicThread->Start();
	//
	bool result = readyAccept();
	if (!result)
		return;
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"Server Start...\n");
}

void neo::network::IOCPServer::StopServer()
{
	for (int i = 0; i < mLogicThreadCount; i++)
		mLogicThreads[i]->Stop();
	mNonLogicThread->Stop();

	for (int i = 0; i < mLogicThreadCount; i++)
		delete mLogicThreads[i];

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
	SocketAddress* clientAddr= new SocketAddress(*remoteAddr);
	session->OnAccept(mClient , clientAddr,mNonLogicThread->GetPacketQueue());

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
	mSessions.push_back(session);

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

	//LogicThread Start
	for (int i = 0; i < mLogicThreadCount; i++)
	{
		mLogicThreads.push_back(new system::LogicThread());
	}


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
