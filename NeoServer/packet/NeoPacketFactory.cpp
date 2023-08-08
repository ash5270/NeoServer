#include"NeoPacketFactory.h"
#include"NeoPacket.h"
std::optional<neo::NeoPacket> neo::packet::PacketFactory::CreateMessage(::io::CodedInputStream& inputStream)
{
	neo::NeoPacket packet;
	inputStream.ReadRaw(&packet.header, sizeof(neo::NeoPacketHeader));
	const void* payloadPtr = nullptr;
	int remainSize = 0;
	//남아있는 사이즈 비교
	inputStream.GetDirectBufferPointer(&payloadPtr, &remainSize);
	if (remainSize < packet.header.packetSize)
		return std::nullopt;

	io::ArrayInputStream payloadArray(payloadPtr, packet.header.packetSize);
	io::CodedInputStream payloadCodeStream(&payloadArray);

	inputStream.Skip(packet.header.packetSize);

	switch (packet.header.packetID)
	{
	case neo::PacketID::PI_C_REQ_LOGIN:
	{
		std::unique_ptr<neo::P_C_REQ_LOGIN> loginReq = std::make_unique<neo::P_C_REQ_LOGIN>();
		loginReq->ParseFromCodedStream(&payloadCodeStream);
		packet.payload = std::move(loginReq);
		return packet;
		break;
	}
	case neo::PacketID::PI_S_RES_LOGIN:
	{
		std::unique_ptr<neo::P_S_RES_LOGIN> loginRes = std::make_unique<neo::P_S_RES_LOGIN>();
		loginRes->ParseFromCodedStream(&payloadCodeStream);
		packet.payload = std::move(loginRes);
		return packet;
		break;
	}

	}
}

std::optional<neo::NeoPacket> neo::packet::PacketFactory::AnalyzerStream(system::InputMemoryStream& stream, const size_t& transfersize)
{
	::io::ArrayInputStream arraySteam(stream.GetStreamPtr() + stream.GetLength(), transfersize);
	::io::CodedInputStream inputStream(&arraySteam);
	neo::NeoPacket packet;
	//header사이즈
	const size_t headerSize = sizeof(neo::NeoPacketHeader);
	inputStream.ReadRaw(&packet.header, headerSize);
	const void* payloadPtr = nullptr;
	int remainSize = 0;
	//남아있는 사이즈 비교
	inputStream.GetDirectBufferPointer(&payloadPtr, &remainSize);
	if (remainSize < packet.header.packetSize)
		return std::nullopt;

	io::ArrayInputStream payloadArray(payloadPtr, packet.header.packetSize);
	io::CodedInputStream payloadCodeStream(&payloadArray);

	inputStream.Skip(packet.header.packetSize);
	stream.AddOffset(headerSize + packet.header.packetSize);

	//패킷 생성
	if (CreatePacket(packet, payloadCodeStream))
	{
		return packet;
	}
	else
	{
		return std::nullopt;
	}
}

bool neo::packet::PacketFactory::CreatePacket(neo::NeoPacket& packet, io::CodedInputStream& codedInputStream)
{
	switch (packet.header.packetID)
	{
	case neo::PacketID::PI_C_REQ_LOGIN:
	{
		std::unique_ptr<neo::P_C_REQ_LOGIN> loginReq = std::make_unique<neo::P_C_REQ_LOGIN>();
		loginReq->ParseFromCodedStream(&codedInputStream);
		packet.payload = std::move(loginReq);
		return true;
	}
	case neo::PacketID::PI_S_RES_LOGIN:
	{
		std::unique_ptr<neo::P_S_RES_LOGIN> loginRes = std::make_unique<neo::P_S_RES_LOGIN>();
		loginRes->ParseFromCodedStream(&codedInputStream);
		packet.payload = std::move(loginRes);
		return true;
	}

	}
}
