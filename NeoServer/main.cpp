#pragma once
#include "network/HttpClient.h"
#include "gameserver/GameServer.h"

#include "system/InputMemoryStream.h"
#include "system/OutputMemoryStream.h"
#include "Common.h"

#include"packetprocess/PacketCommon.h"
#include"manager/ChannelManager.h"
#include"gameserver/LogicThread.h"

#include"gameobject/MapManager.h"
#include"gameobject/MonsterManager.h"

#include"database/DataBaseManager.h"
//#include"Remotery.h"


#include <locale>
#include <json.hpp>

#include<chrono>
#include<iostream>



using namespace neo::packet::process;
using namespace neo::object;
using namespace neo::server;
using namespace std;

int main()
{
//Remotery* rmt;
//	rmt_CreateGlobalInstance(&rmt);


	using namespace  neo::network;
	GameServer server;
	server.InitializeServer(45699);
	server.InitPacketThread();
	server.StartServer();

	ChannelManager::GetInstance().Init();
	
	LoginProcess* login = new LoginProcess();
	ChannelProcess* channel = new ChannelProcess();
	UtilProcess* util = new UtilProcess();

	server.GetChannelThread()[0]->GetPacketProcess()->RegisterProcess(PacketID::PI_C_REQ_LOGIN,
		std::bind(&LoginProcess::Process,login,std::placeholders::_1) );

	server.GetChannelThread()[0]->GetPacketProcess()->RegisterProcess(PacketID::PI_S_NOTIFY_CHANNEL_INFO,
		std::bind(&ChannelProcess::Process, channel, std::placeholders::_1));

	server.GetChannelThread()[0]->GetPacketProcess()->RegisterProcess(PacketID::PI_C_REQ_CHANNEL_REGISTER,
		std::bind(&ChannelProcess::Process, channel, std::placeholders::_1));

	//channel 1
	auto objectManager = dynamic_cast<LogicThread*>(server.GetChannelThread()[1].get())->GetObjectManager();
	CharacterProcess* character = new CharacterProcess(0, objectManager);
	EventProcess* event = new EventProcess(0,objectManager);

	//mapManager
	std::shared_ptr<GameObject> mapMamanager = std::make_shared<MapManager>();
	auto objManagerPtr = objectManager.lock();
	objManagerPtr->RegisterObject(L"mapManager", mapMamanager);
	
	server.GetChannelThread()[1]->GetPacketProcess()->RegisterProcess(PacketID::PI_C_UPDATE_CHAR_POSITION,
		std::bind(&CharacterProcess::Process, character, std::placeholders::_1));
	
	server.GetChannelThread()[1]->GetPacketProcess()->RegisterProcess(PacketID::PI_C_REQ_CHARACTER_REGISTER,
		std::bind(&CharacterProcess::Process, character, std::placeholders::_1));

	server.GetChannelThread()[1]->GetPacketProcess()->RegisterProcess(PacketID::PI_C_NOTIFY_PING,
		std::bind(&UtilProcess::Process, util, std::placeholders::_1));

	server.GetChannelThread()[1]->GetPacketProcess()->RegisterProcess(PacketID::PI_C_NOTIFY_MAP_REGISTER,
		std::bind(&CharacterProcess::Process, character, std::placeholders::_1));

	server.GetChannelThread()[1]->GetPacketProcess()->RegisterProcess(PacketID::PI_C_REQ_CHARACTER_MAP_UNREGISTER,
		std::bind(&CharacterProcess::Process, character, std::placeholders::_1));

	server.GetChannelThread()[1]->GetPacketProcess()->RegisterProcess(PacketID::PI_C_REQ_ATTACK_EVENT,
		std::bind(&EventProcess::Process, event, std::placeholders::_1));

	server.GetChannelThread()[1]->GetPacketProcess()->RegisterProcess(PacketID::PI_C_REQ_ATTACK_RANGE_EVENT,
		std::bind(&EventProcess::Process, event, std::placeholders::_1));

	server.GetChannelThread()[1]->GetPacketProcess()->RegisterProcess(PacketID::PI_C_REQ_ATTACK_RANGE_HIT_EVENT,
		std::bind(&EventProcess::Process, event, std::placeholders::_1));

	server.GetChannelThread()[1]->GetPacketProcess()->RegisterProcess(PacketID::PI_C_NOTIFY_RESPAWN,
		std::bind(&EventProcess::Process, event, std::placeholders::_1));
	
	while (true)
	{
		server.UpdateServer();
	}

	ChannelManager::GetInstance().Stop();
	server.StopServer();
	//rmt_DestroyGlobalInstance(rmt);
	return 0;
}