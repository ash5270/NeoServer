#include "PacketAnalyzeAndCreate.h"
#include"Packet.h"
#include"../system/NeoLog.h"
Packet* neo::packet::PacketFactory::Create(system::InputMemoryStream& stream)
{
	//PACKET ID
	int32_t id = 0;
	stream.Read(id);
	//PACKET SIZE
	int32_t size = 0;
	stream.Read(size);
	//Channel
	int32_t channel = 0;;
	stream.Read(channel);

	Packet* packet = PacketCreate((PacketID)id);
	packet->Channel = static_cast<Channel>(channel);
	return packet;
}

Packet* neo::packet::PacketFactory::PacketCreate(const PacketID& id)
{
	Packet* packet;
	switch (id)
	{
	case PacketID::PI_S_RES_LOGIN:
		packet = new P_S_RES_LOGIN();
		break;

	case PacketID::PI_C_REQ_LOGIN:
		packet = new P_C_REQ_LOGIN();
		break;

	case PacketID::PI_C_REQ_CHANNEL_REGISTER:
		packet = new P_C_REQ_CHANNEL_REGISTER();
		break;

	case PacketID::PI_S_RES_CHANNEL_REGISTER:
		packet = new P_S_RES_CHANNEL_REGISTER();
		break;

	case PacketID::PI_S_NOTIFY_CHANNEL_INFO:
		packet = new P_S_NOTIFY_CHANNEL_INFO();
		break;

	case PacketID::PI_C_NOTIFY_CHANNEL_UNREGISTER:
		packet = new P_C_NOTIFY_CHANNEL_UNREGISTER();
		break;

	default:
		packet = nullptr;
		break;
	}

	return packet;
}


Packet* neo::packet::PacketAnalyzeAndCreate::Analyzer(system::InputMemoryStream& stream)
{
	//packet 분석 후 생성
	auto packet = PacketFactory::GetInstance().Create(stream);
	if (packet == nullptr)
	{
		LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"packet nullptr...\n");
		return nullptr;
	}
	
	packet->Deserialize(stream);
	return packet;
}
