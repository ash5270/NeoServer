#include "LoginProcess.h"
#include"../manager/ChannelManager.h"


#include<json.h>
#include<packet/Packet.h>
#include<system/NeoLog.h>
#include<vector>

neo::packet::process::LoginProcess::LoginProcess()
{
	//mRedis = new sw::redis::Redis(option);

}

neo::packet::process::LoginProcess::~LoginProcess()
{
	
}

bool neo::packet::process::LoginProcess::Login(const P_C_REQ_LOGIN* loginData)
{
	auto idStr = std::string().assign(loginData->id.begin(),
		loginData->id.end());
	//Ű���� c_str ���� �ؾ���


	sw::redis::Redis mRedis("tcp://192.168.123.104:6379");

	auto uuid = mRedis.get(idStr.c_str());
	if (uuid != nullopt && UuidCheck(loginData->uuid.get(), (*uuid).c_str()))
	{
		LOG_PRINT(LOG_LEVEL::LOG_INFO, L"%s Login Success\n", loginData->id.c_str());
		return true;
	}
	else
	{
		LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"%s Login Fail info\n", loginData->id.c_str());
		return false;
	}
}

bool neo::packet::process::LoginProcess::UuidCheck(const char* lUuid, const char* rUuid)
{
	for (int i = 0; i < 16; i++)
	{
		if (lUuid[i] != rUuid[i])
		{
			return false;
		}
	}
	return true;
}

void neo::packet::process::LoginProcess::SendResultMsg(const packet::PacketObejct* packet, const std::wstring& msg, const int32_t& statusCode)
{
	P_S_RES_LOGIN respone;
	respone.msg = msg;
	respone.status = statusCode;

	packet->session->SendPacket(respone);
}

void neo::packet::process::LoginProcess::Process(packet::PacketObejct* packet)
{
	if (packet->packet->GetID() == PacketID::PI_C_REQ_LOGIN)
	{
		auto loginPacket = dynamic_cast<P_C_REQ_LOGIN*>(packet->packet);
		if (Login(loginPacket))
		{
			SendResultMsg(packet, L"Login Success", 202);
			P_S_NOTIFY_CHANNEL_INFO channelInfo;
			channelInfo.json = object::ChannelManager::GetInstance().GetJsonAllChannelInfo();
			packet->session->SendPacket(channelInfo);
		}
		else {
			SendResultMsg(packet, L"Login Failed", 404);
		}
	}

	delete packet->packet;
}

