#pragma once
#include<system/InputMemoryStream.h>
#include<system/OutputMemoryStream.h>
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
	//Ping
	PI_C_NOTIFY_PING = 1,
	PI_S_NOTIFY_PING = 2,
	
	//player
	PI_C_UPDATE_CHAR_POSITION = 100,
	PI_S_NOTIFY_CHAR_POSITION = 200,
	PI_S_NOTIFY_CHAR_UDPATE = 201,
	//
	PI_S_NOTIFY_MONSTER_POSITION = 300,

	//로그인 요청
	PI_S_RES_LOGOUT=998,
	PI_C_REQ_LOGOUT = 999,
	PI_C_REQ_LOGIN = 1000,
	//로그인 응답
	PI_S_RES_LOGIN = 2000,
	//채널 등록 요청
	PI_C_REQ_CHANNEL_REGISTER = 3000,
	//채널 등록 응답 
	PI_S_RES_CHANNEL_REGISTER = 3001,
	PI_C_NOTIFY_CHANNEL_UNREGISTER = 3002,
	PI_C_NOTIFY_MAP_REGISTER = 3003,

	//채널에 캐릭터 등록 요청
	PI_C_REQ_CHARACTER_REGISTER = 3100,
	PI_S_RES_CHARACTER_REGISTER = 3101,

	PI_C_REQ_CHARACTER_MAP_UNREGISTER = 3102,
	PI_S_NOTIFY_CHARACTER_MAP_UNREGISTER = 3103,

	PI_S_NOTIFY_MONSTER_MAP_REGISTER = 3110,
	PI_S_NOTIFY_MONSTER_MAP_UNREGISTER = 3111,

	PI_S_NOTIFY_ATTACK_ANI_EVENT = 3150,

	//채널에 등록된 유저 정보 전달
	PI_S_NOTIFY_CHARACTER_INFO = 3202,

	//몬스터 정보 전달
	PI_S_NOTIFY_MONSTER_INFO = 3300,
	//어택 에니메이션 전달

	//이벤트 패킷
	PI_C_REQ_ATTACK_RANGE_HIT_EVENT=3497,
	PI_S_NOTIFY_EFFECT_EVENT = 3498,
	PI_C_REQ_ATTACK_RANGE_EVENT = 3499,

	PI_C_REQ_ATTACK_EVENT = 3500,
	PI_S_RES_ATTACK_EVENT = 3501,
	PI_S_NOTIFY_ATTACK_EVENT = 3502,

	//오브젝트 업데이트
	PI_NOTIFY_OBJECT_UPDATE = 3510,

	//채널 정보 송신
	PI_S_NOTIFY_CHANNEL_INFO = 3999,
};


class Packet
{
public:
	virtual PacketID GetID() { return (PacketID)0; };
	virtual int32_t GetSize() { return 0; };
	Channel Channel = Channel::ALL;
	virtual void Serialize(OutputMemoryStream& buffer) {};
	virtual void Deserialize(InputMemoryStream& buffer) {};
	//header
	void HeaderSerialize(OutputMemoryStream& buffer)
	{
		buffer.Write((int)GetID());
		buffer.Write((int)GetSize());
		buffer.Write((int)Channel);
	}
};