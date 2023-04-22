#pragma once
#include"../system/OutputMemoryStream.h"
#include"../system/InputMemoryStream.h"
using namespace neo::system;

enum class Channel : int32_t
{
	//
	ALL = 0,
	ONE = 1,
	TWO = 2,
};


enum class PacketID : int32_t
{
	//로그인 요청
	PI_C_REQ_LOGIN = 1000,
	//로그인 응답
	PI_S_RES_LOGIN = 2000,
	//채널 등록 요청
	PI_C_REQ_CHANNEL_REGISTER = 3000,
	//채널 등록 응답 
	PI_S_RES_CHANNEL_REGISTER = 3001,
	PI_C_NOTIFY_CHANNEL_UNREGISTER=3002,
	//채널 정보 송신
	PI_S_NOTIFY_CHANNEL_INFO = 3999,
};

class Packet
{
public:
	virtual PacketID GetID() = 0;
	virtual int32_t GetSize() = 0;
	Channel Channel = Channel::ALL;
	virtual void Serialize(OutputMemoryStream& buffer) = 0;
	virtual void Deserialize(InputMemoryStream& buffer) = 0;
	//header
	void HeaderSerialize(OutputMemoryStream& buffer)
	{
		buffer.Write((int)GetID());
		buffer.Write(GetSize());
		buffer.Write(Channel);
	}
};