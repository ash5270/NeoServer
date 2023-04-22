#pragma once
#include <sw/redis++/redis++.h>
#include "network/HttpClient.h"
#include "gameserver/GameServer.h"

#include "system/InputMemoryStream.h"
#include "system/OutputMemoryStream.h"
#include "Common.h"

#include"packetprocess/PacketCommon.h"
#include"manager/ChannelManager.h"
#include <locale>
#include <json.h>


using namespace neo::packet::process;
using namespace neo::object;
using namespace neo::server;
using namespace std;


int main()
{
	using namespace  neo::network;
	GameServer server;
	server.InitializeServer(45699);
	server.StartServer();

	ChannelManager::GetInstance().Init();
	ChannelManager::GetInstance().AddChannel(L"Krin");
	ChannelManager::GetInstance().AddChannel(L"Rupso");

	LoginProcess* login = new LoginProcess();
	ChannelProcess* channel = new ChannelProcess();

	server.GetNonLogicThread()->GetPacketProcess()->RegisterProcess(PacketID::PI_C_REQ_LOGIN,
		std::bind(&LoginProcess::Process,login,std::placeholders::_1) );

	server.GetNonLogicThread()->GetPacketProcess()->RegisterProcess(PacketID::PI_S_NOTIFY_CHANNEL_INFO,
		std::bind(&ChannelProcess::Process, channel, std::placeholders::_1));

	server.GetNonLogicThread()->GetPacketProcess()->RegisterProcess(PacketID::PI_C_REQ_CHANNEL_REGISTER,
		std::bind(&ChannelProcess::Process, channel, std::placeholders::_1));

	while (true)
	{
		server.UpdateServer();
	}

	ChannelManager::GetInstance().Stop();
	server.StopServer();

	return 0;
}