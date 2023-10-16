#include "CharacterProcess.h"
#include"../manager/ChannelManager.h"
#include"../gameobject/MapManager.h"

#include"../database/DataBaseManager.h"
#include"../gameobject/PlayerObject.h"
#include<cppconn/driver.h>

neo::packet::process::CharacterProcess::CharacterProcess(const int32_t& channel, std::weak_ptr<server::ObjectManager> objectManager) :mObjectManager(objectManager), mChannel(channel)
{
	 db = db::DataBaseManager::GetInstance().GetNewConnection();
}

neo::packet::process::CharacterProcess::~CharacterProcess()
{
	mysql_close(db);
}

void neo::packet::process::CharacterProcess::Process(packet::PacketObject* packet)
{
	if (packet->packet->GetID() == PacketID::PI_C_UPDATE_CHAR_POSITION)
	{
		UpdatePosition(packet);
	}
	else if (packet->packet->GetID() == PacketID::PI_C_REQ_CHARACTER_REGISTER)
	{
		RegisterUser(packet);
	}
	else if (packet->packet->GetID() == PacketID::PI_C_NOTIFY_MAP_REGISTER)
	{
		UpdatePlayerMap(packet);
	}
	else if (packet->packet->GetID() == PacketID::PI_C_REQ_CHARACTER_MAP_UNREGISTER)
	{
		MapUnregisterUser(packet);
	}
	
	
	delete packet->packet;
}

void neo::packet::process::CharacterProcess::UpdatePosition(const packet::PacketObject* packet)
{
	auto posPacket = dynamic_cast<P_C_UPDATE_CHAR_POSITION*>(packet->packet);

	auto manager = mObjectManager.lock();
	auto weakObject = manager->GetGameObject(posPacket->name);
	auto gameObject = weakObject.lock();

	gameObject->transform.position = posPacket->position;
	gameObject->transform.veloctiy = posPacket->velocity;
}

void neo::packet::process::CharacterProcess::UpdatePlayerMap(const packet::PacketObject* packet)
{
	/*auto registerPacket = dynamic_cast<P_C_NOTIFY_MAP_REGISTER*>(packet->packet);
	auto manager = mObjectManager.lock();
	auto Object = manager->GetGameObject(L"mapManager").lock();
	auto mapMananger =
		std::dynamic_pointer_cast<object::MapManager>(Object);
	
	if (registerPacket->isRegister)
	{
		auto playerObject = manager->GetGameObject(registerPacket->name).lock();
		mapMananger->RegisterUser(registerPacket->mapCount,playerObject);
		LOG_PRINT(LOG_LEVEL::LOG_INFO, L"player map insert name : %s, map count : %d\n", registerPacket->name.c_str(), registerPacket->mapCount);
	}
	else
	{
		auto playerObject = manager->GetGameObject(registerPacket->name).lock();
		mapMananger->UnregisterUser(registerPacket->mapCount,playerObject->Name);
		manager->UnregisterObject(playerObject->Name);
	}*/
}

void neo::packet::process::CharacterProcess::RegisterUser(const packet::PacketObject* packet)
{
	auto registerPacket = dynamic_cast<P_C_REQ_CHARACTER_REGISTER*>(packet->packet);
	auto manager = mObjectManager.lock();

	std::shared_ptr<object::GameObject> player = std::make_shared<object::PlayerObject>();
	manager->RegisterObject(registerPacket->name, player, packet->session);
	MapRegisterUser(packet);

	if (db)
	{
		MYSQL_RES* res;
		MYSQL_ROW row;

		std::string name = std::string().assign(registerPacket->name.begin(), registerPacket->name.end());
		auto buffer = std::make_unique<char[]>(100);
		auto len = sprintf_s(buffer.get(), 100, "SELECT char_level,char_exp,char_hp,char_max_hp FROM user_character where char_name = '%s'", name.c_str());
		int result = mysql_query(db, buffer.get());
		if (!result)
		{

			res = mysql_store_result(db);
			row = mysql_fetch_row(res);
			int32_t char_level = std::stoi(row[0]);
			int32_t char_exp = std::stoi(row[1]);
			int32_t char_hp = std::stoi(row[2]);
			int32_t char_max_hp = std::stoi(row[3]);
			LOG_PRINT(LOG_LEVEL::LOG_INFO, L"name : %s, level : %d, exp : %d, hp : %d\n",registerPacket->name.c_str(), char_level, char_exp, char_hp);
		
			auto player= manager->GetGameObject(registerPacket->name).lock();
			auto playerPtr = std::dynamic_pointer_cast<object::PlayerObject>(player);
			playerPtr->InitData(char_max_hp,char_hp, char_exp, char_level);
		}
	}
	mysql_free_result(res);
}

void neo::packet::process::CharacterProcess::MapRegisterUser(const packet::PacketObject* packet)
{
	auto registerPacket = dynamic_cast<P_C_REQ_CHARACTER_REGISTER*>(packet->packet);
	auto manager = mObjectManager.lock();

	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"map info enter map count : %d\n",registerPacket->mapID);

	if(mMapmanager.expired())
		mMapmanager = std::dynamic_pointer_cast<object::MapManager>(manager->GetGameObject(L"mapManager").lock());
	auto map = mMapmanager.lock()->GetMapData(registerPacket->mapID).lock();

	P_S_RES_CHARACTER_REGISTER respone;
	respone.status = 200;
	respone.msg = L"success";
	respone.json = map->GetJsonGameObject(registerPacket->name);
	packet->session->SendPacket(respone);

	auto monsterManagerPtr =
		map->MonsterManager.lock();

	P_S_NOTIFY_MONSTER_INFO monsterInfos;
	monsterInfos.monsterCode = monsterManagerPtr->monsterCode;
	monsterInfos.msg = L"success";
	monsterInfos.json = monsterManagerPtr->GetJsonGameObject();
	packet->session->SendPacket(monsterInfos);

	P_S_NOTIFY_CHARACTER_INFO notify;
	notify.name = registerPacket->name;

	auto mapPtr = mMapmanager.lock()->GetMapData(registerPacket->mapID).lock();
	for (const auto& player : mapPtr->GetInMapPlayers())
	{
		auto copyPacket = notify;
		if (player.second.lock()->Session != packet->session)
			player.second.lock()->Session->SendPacket(copyPacket);
	}

	auto player = manager->GetGameObject(registerPacket->name);
	mMapmanager.lock()->RegisterUser(registerPacket->mapID,player);
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"register map id : %d, user name : %s\n", registerPacket->mapID, registerPacket->name.c_str());
}

void neo::packet::process::CharacterProcess::MapUnregisterUser(const packet::PacketObject* packet)
{
	auto manager = mObjectManager.lock();
	auto charPacket = dynamic_cast<P_C_REQ_CHARACTER_MAP_UNREGISTER*>(packet->packet);

	if (mMapmanager.expired())
		mMapmanager = std::dynamic_pointer_cast<object::MapManager>(manager->GetGameObject(L"mapManager").lock());
	auto map = mMapmanager.lock()->GetMapData(charPacket->mapID).lock();

	map->UnReigsterObject(charPacket->name);

	P_S_NOTIFY_CHARACTER_MAP_UNREGISTER unregister;
	unregister.name = charPacket->name;
	unregister.mapID = charPacket->mapID;

	for (const auto& player : map->GetInMapPlayers())
	{
		auto copyPacket = unregister;
		if (player.second.lock()->Session != packet->session)
			player.second.lock()->Session->SendPacket(copyPacket);
	}
}
