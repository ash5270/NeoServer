#pragma once
//network lib
#include"BasePacketProcess.h"
#include"../packet/PacketObject.h"
#include"../packet/Packet.h"
//reids
//#include <sw/redis++/redis++.h>
//#include <hiredis/hiredis.h>

namespace neo::packet::process
{
	class LoginProcess :public BasePacketProcess
	{
	public:
		LoginProcess();
		~LoginProcess();

	public:
		// BasePacketProcess을(를) 통해 상속됨
		virtual void Process(packet::PacketObject* packet) override;
	private:
		bool Login(const P_C_REQ_LOGIN* loginData);
		bool UuidCheck(const char* lUuid, const char* rUuid);
		void SendResultMsg(const packet::PacketObject* packet,const std::wstring& msg, const int32_t& code);
	};
}
