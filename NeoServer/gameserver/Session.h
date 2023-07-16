#pragma once
#include<network/IOCPSession.h>
#include<LockFreeQueue.h>
#include"../packet/Packet.h"
using namespace neo::network;

namespace neo::packet {
	class PacketObject;
}

namespace neo::server {
	
	class GameServer;
	class Session : public IOCPSession
	{
	public:
		Session();
		~Session();
	public:
		bool OnAccept(TCPSocket* socket, SocketAddress* addrInfo) override;
		void SetGameServer(GameServer* gameServer);

		void OnSend(size_t transferSize) override;
		void OnRecv(size_t transferSize) override;
		void OnClose() override;

		void SendPacket(Packet& packet);
	
		void SetChannelId(const int& id)
		{
			mChannelId = id;
		}

		int GetChannelId() const
		{
			return mChannelId;
		}

	private:
		int mChannelId =0;
		//recv하고 남은 offset
		size_t mRecvOffset =0 ;
	private:
		GameServer* mGameServer;
		int count = 0;
	};
}