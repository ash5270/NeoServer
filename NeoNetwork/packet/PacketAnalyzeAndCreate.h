#pragma once
#include "../Singleton.h"
#include"PacketID.h"
#include"../system/InputMemoryStream.h"

namespace neo::packet {
	class PacketFactory :public Singleton<PacketFactory>
	{
	public:
		Packet* Create(system::InputMemoryStream& stream);
	private: 
		Packet* PacketCreate(const PacketID& id);
	};

	class PacketAnalyzeAndCreate : public Singleton<PacketAnalyzeAndCreate>
	{
	public:
		Packet* Analyzer(system::InputMemoryStream& stream);
	};
}



