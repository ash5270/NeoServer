#pragma once
#include<Singleton.h>
#include<system/InputMemoryStream.h>
#include"NeoPacket.h"

using namespace google::protobuf;

namespace neo::packet {
	class PacketFactory : public neo::Singleton<PacketFactory>
	{
	public:
		std::optional<neo::NeoPacket> CreateMessage(::io::CodedInputStream& inputStream);
		std::optional<neo::NeoPacket> AnalyzerStream(system::InputMemoryStream& stream, const size_t& transfersize);
	private:
		bool CreatePacket(neo::NeoPacket& packet, io::CodedInputStream& codedInputStream);
	};
}