//#pragma once
//#include<system/InputMemoryStream.h>
//#include<Singleton.h>
//
//#include"PacketID.h"
//
//namespace neo::packet {
//	class PacketFactory :public Singleton<PacketFactory>
//	{
//	public:
//		Packet* Create(system::InputMemoryStream& stream,const size_t& transferSize);
//	private:
//		Packet* PacketCreate(const PacketID& id);
//	};
//
//	class PacketAnalyzeAndCreate : public Singleton<PacketAnalyzeAndCreate>
//	{
//	public:
//		Packet* Analyzer(system::InputMemoryStream& stream,const size_t& transferSize);
//	};
//}