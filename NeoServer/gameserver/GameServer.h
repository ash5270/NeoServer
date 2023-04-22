#pragma once
#include<network/IOCPServer.h>

using namespace neo::network;

namespace neo::server {
	class GameServer : public IOCPServer
	{
	public:
		GameServer();
		~GameServer();

		void UpdateServer() override;
			
		
	};
}

