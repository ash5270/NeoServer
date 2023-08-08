#pragma once
//network lib
#include"BasePacketProcess.h"
#include"../packet/NeoPacket.h"
//reids
#include <sw/redis++/redis++.h>
#include <hiredis/hiredis.h>

namespace neo::packet::process
{
	class LoginProcess :public BasePacketProcess
	{
	public:
		LoginProcess();
		~LoginProcess();

	public:
		// BasePacketProcess을(를) 통해 상속됨
		virtual void Process(const neo::PacketObjPtr& packetObjPtr ) override;
	private:
		bool Login(const std::unique_ptr<P_C_REQ_LOGIN>& packet);
		bool UuidCheck(const char* lUuid, const char* rUuid);
		void SendResultMsg(const std::unique_ptr<neo::server::Session>& session,const std::wstring& msg, const PacketResult& statusCode);
		void SendChannelInfo(const std::unique_ptr<neo::server::Session>& session);
	private:
		sw::redis::Redis* mRedis;
	};
}
