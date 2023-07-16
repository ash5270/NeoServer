#pragma once
#include<network/IOCPServer.h>
#include"../BasePacketThread.h"
#include"SessionPool.h"
#include<chrono>
using namespace neo::network;

namespace neo::server {
	class GameServer : public IOCPServer
	{
	public:
		GameServer();
		~GameServer();

	public:
		void InitPacketThread();
		void UpdateServer() override;
		//Session create
		IOCPSession* OnAccept(TCPSocket* socket, SocketAddress* addrInfo) override;
		//get channel thread 
		const std::vector<std::unique_ptr<system::BasePacketThread>>& GetChannelThread() const
		{
			return mChannelThreads;
		}
	private:
		//Session pool
		std::unique_ptr<SessionPool> mSessionPool;
		//connected session
		std::list<std::shared_ptr<Session>> mConnectSessions;

		//channel thread 
		std::vector<std::unique_ptr<system::BasePacketThread>> mChannelThreads;
		//packet queue
		const int mLogicThreadCount = 2;
		
		Session* ses;
	};
}

