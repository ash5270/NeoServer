//프로토 버퍼에 연속된 데이터를 사용하기 위해 
//header따로 만들어둠
#pragma once
#include<memory>
#include"Packet.pb.h"
#include"../gameserver/Session.h"

namespace neo
{
	namespace server
	{
		class Session;
	}

	struct NeoPacketHeader
	{
		PacketID packetID = PacketID::PI_TEST_PACKET;
		int32_t packetSize = 0;
		int32_t channel = 0;
	};

	struct NeoPacket
	{
		NeoPacketHeader header;
		std::unique_ptr<google::protobuf::Message> payload;
	};

	struct PacketObj
	{
		NeoPacket packet;
		std::unique_ptr<server::Session> session;
	};

	using PacketObjPtr = std::unique_ptr<PacketObj>;

	NeoPacket CreateNewPacket(const PacketID& packetID,google::protobuf::Message* message)
	{
		NeoPacket packet;
		packet.header.packetID = packetID;
		packet.payload = std::unique_ptr<google::protobuf::Message>(message);
		return packet;
	}
}
