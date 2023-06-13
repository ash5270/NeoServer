#include "GameServer.h"
#include "LogicThread.h"
#include "PacketProcessThread.h"
neo::server::GameServer::GameServer() : IOCPServer()
{
	//Thread Init
	mChannelThreads.push_back(std::make_unique<system::PacketProcessThread>(0));
	for (int i = 1; i < mLogicThreadCount + 1; i++)
	{
		mChannelThreads.push_back(std::make_unique<system::LogicThread>(i));
	}

	//session pool create
	mSessionPool = std::make_unique<SessionPool>(1024);

	
}

neo::server::GameServer::~GameServer()
{
	for (int i = 0; i < mChannelThreads.size() ; i++)
	{
		mChannelThreads[i]->Stop();
	}
}

void neo::server::GameServer::InitPacketThread()
{
	for (int i = 0; i < mChannelThreads.size(); i++)
	{
		mChannelThreads[i]->Start();
	}
}

void neo::server::GameServer::UpdateServer()
{
	
}

IOCPSession* neo::server::GameServer::OnAccept(TCPSocket* socket, SocketAddress* addrInfo)
{
	//새로운 접속이 들어왔을때 
	//auto session = new Session();
	auto session = mSessionPool->GetSharedObject();
	auto sessionPtr = session.get();
	auto id = static_cast<int>(Channel::ALL);
	ses = sessionPtr;

	session->SetGameServer(this);
	session->OnAccept(socket, addrInfo);
	mConnectSessions.push_back(session);
	return sessionPtr;
}
