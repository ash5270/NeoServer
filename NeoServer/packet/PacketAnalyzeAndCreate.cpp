//#include "PacketAnalyzeAndCreate.h"
//#include"Packet.h"
//#include<system/NeoLog.h>
//Packet* neo::packet::PacketFactory::Create(system::InputMemoryStream& stream,const size_t& transferSize)
//{
//	//header 사이즈 자체가 읽을 수 없는경우
//	if (stream.GetLength() + sizeof(int32_t)+ sizeof(int32_t) + sizeof(int32_t) >transferSize)
//	{
//		LOG_PRINT(LogType::LOG_INFO, L"Error HEAER size failed...\n");
//		return nullptr;
//	}
//
//	//PACKET ID
//	int32_t id = 0;
//	stream.Read(id);
//	//PACKET SIZE
//	int32_t size = 0;
//	stream.Read(size);  //여기서 사이즈 체크를 해줘야할듯 
//	//Channel
//	int32_t channel = 0;
//	stream.Read(channel);
//	//size check 
//
//	if (stream.GetLength() + size > transferSize)
//	{
//		auto lastOffset = stream.GetLength() - sizeof(id) - sizeof(size) - sizeof(channel);
//		LOG_PRINT(LogType::LOG_INFO, L"Error packet size failed... last offet = %d\n",lastOffset);
//		stream.SetOffset(lastOffset);
//		return nullptr;
//	}
//	else
//	{
//		Packet* packet = PacketCreate((PacketID)id);
//		packet->Channel = static_cast<Channel>(channel);
//		return packet;
//	}
//}
//
//Packet* neo::packet::PacketFactory::PacketCreate(const PacketID& id)
//{
//	Packet* packet;
//	switch (id)
//	{
//	case PacketID::PI_S_RES_LOGIN:
//		packet = new P_S_RES_LOGIN();
//		break;
//
//	case PacketID::PI_C_REQ_LOGIN:
//		packet = new P_C_REQ_LOGIN();
//		break;
//
//	case PacketID::PI_C_REQ_CHANNEL_REGISTER:
//		packet = new P_C_REQ_CHANNEL_REGISTER();
//		break;
//
//	case PacketID::PI_S_RES_CHANNEL_REGISTER:
//		packet = new P_S_RES_CHANNEL_REGISTER();
//		break;
//
//	case PacketID::PI_S_NOTIFY_CHANNEL_INFO:
//		packet = new P_S_NOTIFY_CHANNEL_INFO();
//		break;
//
//	case PacketID::PI_C_NOTIFY_CHANNEL_UNREGISTER:
//		packet = new P_C_NOTIFY_CHANNEL_UNREGISTER();
//		break;
//
//	case PacketID::PI_C_UPDATE_CHAR_POSITION:
//		packet = new P_C_UPDATE_CHAR_POSITION();
//		break;
//
//	case PacketID::PI_C_REQ_CHARACTER_REGISTER:
//		packet = new P_C_REQ_CHARACTER_REGISTER();
//		break;
//
//	case PacketID::PI_S_RES_CHARACTER_REGISTER:
//		packet = new P_S_RES_CHARACTER_REGISTER();
//		break;
//
//	case PacketID::PI_C_NOTIFY_MAP_REGISTER:
//		packet = new P_C_NOTIFY_MAP_REGISTER();
//		break;
//
//	case PacketID::PI_C_REQ_ATTACK_EVENT:
//		packet = new P_C_REQ_ATTACK_EVENT();
//		break;
//
//	case PacketID::PI_S_RES_ATTACK_EVENT:
//		packet = new P_S_RES_ATTACK_EVENT();
//		break;
//
//	case PacketID::PI_S_NOTIFY_CHARACTER_MAP_UNREGISTER:
//		packet = new P_S_NOTIFY_CHARACTER_MAP_UNREGISTER;
//		break;
//
//	case PacketID::PI_C_REQ_CHARACTER_MAP_UNREGISTER:
//		packet = new P_C_REQ_CHARACTER_MAP_UNREGISTER;
//		break;
//
//	case PacketID::PI_C_REQ_ATTACK_RANGE_EVENT:
//		packet = new P_C_REQ_ATTACK_RANGE_EVENT;
//		break;
//
//	case PacketID::PI_C_REQ_ATTACK_RANGE_HIT_EVENT:
//		packet = new P_C_REQ_ATTACK_RANGE_HIT_EVENT;
//		break;
//
//	case PacketID::PI_C_NOTIFY_PING:
//		packet = new P_C_NOTIFY_PING;
//		break;
//
//	case PacketID::PI_C_NOTIFY_RESPAWN:
//		packet = new P_C_NOTIFY_RESPAWN;
//		break;
//
//	default:
//		packet = nullptr;
//		break;
//	}
//
//	return packet;
//}
//
//
//Packet* neo::packet::PacketAnalyzeAndCreate::Analyzer(system::InputMemoryStream& stream,const size_t& transferSize)
//{
//	//packet 분석 후 생성
//	auto packet = PacketFactory::GetInstance().Create(stream,transferSize);
//	if (packet == nullptr)
//	{
//		LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"packet nullptr...\n");
//		return nullptr;
//	}
//	
//	packet->Deserialize(stream);
//	return packet;
//}
