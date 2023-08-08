#include "LoginProcess.h"
#include"../manager/ChannelManager.h"
#include"../packet/Packet.h"
#include<sw/redis++/redis.h>
#include<json.hpp>
#include<system/NeoLog.h>
#include<vector>
#include<UtfToUnicode.h>

neo::packet::process::LoginProcess::LoginProcess()
{
	//mRedis = new sw::redis::Redis("tcp://192.168.123.104:6379");
	//sw::redis::Redis mRedis();
}

neo::packet::process::LoginProcess::~LoginProcess()
{

}

bool neo::packet::process::LoginProcess::Login(const std::unique_ptr<P_C_REQ_LOGIN>& packet)
{
	auto idWStr = Utf8ToUnicode(packet->id());
	//키값을 c_str 으로 해야함
	auto uuid = mRedis->get(packet->id().c_str());
	if (uuid != nullopt && UuidCheck(packet->uuid().c_str(), (*uuid).c_str())) {
		LOG_PRINT(LOG_LEVEL::LOG_INFO, L"%s Login Success\n", packet->id().c_str());
		return true;
	}
	else
	{
		LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"%s Login Fail info\n", packet->id().c_str());
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

void neo::packet::process::LoginProcess::SendResultMsg(const std::unique_ptr<neo::server::Session>& session,
	const std::wstring& msg,
	const PacketResult& statusCode)
{
	//utf-8로 인코딩
	auto resultMsg = UnicodeToUtf8(msg);
	if(!resultMsg.has_value())
	{
		LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"Unicode parsing error, %s\n", __FUNCTION__);
		return;
	}
	
	//프로토 버퍼 생성
	std::unique_ptr<P_S_RES_LOGIN> responePayload = std::make_unique<P_S_RES_LOGIN>();
	responePayload->set_msg(resultMsg.value());
	responePayload->set_result(statusCode);
	//패킷 생성
	NeoPacket respone = CreateNewPacket(PI_S_RES_LOGIN,responePayload.release());
	session->SendPacket(respone);
}

void neo::packet::process::LoginProcess::SendChannelInfo(const std::unique_ptr<neo::server::Session>& session)
{
	std::unique_ptr<P_S_NOTIFY_CHANNEL_INFO> notifyPayload = std::make_unique<P_S_NOTIFY_CHANNEL_INFO>();
	for(const auto& ptr :object::ChannelManager::GetInstance().GetChannels())
	{
		auto channel = notifyPayload->add_chaanelinfos();
		channel->set_name(ptr->GetName());
		channel->set_channelid(ptr->GetNumber());
		channel->set_usercount(static_cast<int32_t>(ptr->GetSesssion().size()));
	}
	NeoPacket respone = CreateNewPacket(PI_S_NOTIFY_CHANNEL_INFO, notifyPayload.release());
	session->SendPacket(respone);
}

void neo::packet::process::LoginProcess::Process(const neo::PacketObjPtr& packetObjPtr)
{
	if (packetObjPtr->packet.header.packetID == PacketID::PI_C_REQ_LOGIN)
	{
		auto castPacket = static_cast<P_C_REQ_LOGIN*>(packetObjPtr->packet.payload.release());
		std::unique_ptr<P_C_REQ_LOGIN> loginPacket(castPacket);
		if (Login(loginPacket))
		{
			SendResultMsg(packetObjPtr->session, L"Login Success", PacketResult::SUCCESS);
			SendChannelInfo(packetObjPtr->session);
		}
		else
		{
			SendResultMsg(packetObjPtr->session, L"Login Failed", PacketResult::FAILED);
		}
	}
}


