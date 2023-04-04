#include "PacketAnalyzeAndCreate.h"
#include"Packet.h"
Packet* neo::packet::PacketFactory::Create(system::InputMemoryStream& stream)
{
	//PACKET ID
	int32_t id = 0;
	stream.Read(id);
	//PACKET SIZE
	int32_t size = 0;
	stream.Read(size);

	Packet* packet = PacketCreate((PacketID)id);
	packet->header.id = id;
	packet->header.size = size;
	return packet;
}

Packet* neo::packet::PacketFactory::PacketCreate(const PacketID& id)
{
	Packet* packet;
	switch (id)
	{
	case PI_S_RES_LOGIN:
		packet = new P_S_RES_LOGIN();
		break;

	case PI_C_REQ_LOGIN:
		packet = new P_C_REQ_LOGIN();
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
	packet->Deserialize(stream);
	return packet;
}
