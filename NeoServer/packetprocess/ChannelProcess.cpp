#include "ChannelProcess.h"
#include "../packet/Packet.h"
neo::packet::process::ChannelProcess::ChannelProcess()
{

}

neo::packet::process::ChannelProcess::~ChannelProcess()
{

}

void neo::packet::process::ChannelProcess::Process(const neo::PacketObjPtr& packetObj)
{
	if (packetObj->packet.header.packetID == PacketID::PI_C_REQ_CHANNEL_REGISTER)
	{
		//채널
		auto castPtr = static_cast<P_C_REQ_CHANNEL_REGISTER*>(packetObj->packet.payload.release());
		std::unique_ptr<P_C_REQ_CHANNEL_REGISTER> registerPacket(castPtr);
		//auto registerPacket = dynamic_cast<P_C_REQ_CHANNEL_REGISTER*>(packet->packet);
		LOG_PRINT(LOG_LEVEL::LOG_INFO, L"id : %s enter channel id : %d\n",
			registerPacket->user().c_str(),
			registerPacket->channelid());

		//채널에 세션 추가
		object::ChannelManager::GetInstance().AddUser(registerPacket->channelid(), packetObj->session);

		P_S_RES_CHANNEL_REGISTER respone;
		respone.channelID = registerPacket->channelID;
		respone.msg = L"success";
		respone.status = 200;
		packet->session->SendPacket(respone);
	}

	delete packet->packet;
}

void neo::packet::process::ChannelProcess::ChannelInfoRespone(const  neo::packet::PacketObject* packet)
{

}

void neo::packet::process::ChannelProcess::ChannelRegister(const  neo::packet::PacketObject* packet)
{

}
