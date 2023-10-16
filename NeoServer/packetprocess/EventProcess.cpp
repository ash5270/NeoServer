#include "EventProcess.h"
#include"../gameobject/MonsterObject.h"
#include"../gameobject/MapManager.h"
#include"../gameobject/PlayerObject.h"

neo::packet::process::EventProcess::EventProcess(const int32_t& channel, std::weak_ptr<server::ObjectManager> objectManager) :mObjectManager(objectManager), mChannel(channel)
{
	 db = db::DataBaseManager::GetInstance().GetNewConnection();
}

neo::packet::process::EventProcess::~EventProcess()
{
	mysql_close(db);
}

void neo::packet::process::EventProcess::Process(packet::PacketObject* packet)
{
	if (packet->packet->GetID() == PacketID::PI_C_REQ_ATTACK_EVENT)
	{
		AttackProcess(packet);
	}
	else if (packet->packet->GetID() == PacketID::PI_C_REQ_ATTACK_RANGE_EVENT)
	{
		RangeAttackProcess(packet);
	}
	else if (packet->packet->GetID() == PacketID::PI_C_REQ_ATTACK_RANGE_HIT_EVENT)
	{
		RangeAttackHit(packet);
	}
	else if (packet->packet->GetID() == PacketID::PI_C_NOTIFY_RESPAWN)
	{
		auto respawnPacket = dynamic_cast<P_C_NOTIFY_RESPAWN*>(packet->packet);
		auto playerPtr = mObjectManager.lock()->GetGameObject(respawnPacket->id);
		if (!playerPtr.expired())
		{
			auto playerObject = std::dynamic_pointer_cast<object::PlayerObject>(playerPtr.lock());
			playerObject->Respwan();
			P_S_NOTIFY_CHAR_UPDATE update;
			update.exp = playerObject->GetEXP();
			update.hp = playerObject->GetHp();
			update.level = playerObject->GetLevel();
			playerObject->Session->SendPacket(update);
		}
		
	}
	delete packet->packet;
}

void neo::packet::process::EventProcess::AttackProcess(packet::PacketObject* packet)
{
	auto attackPacket = dynamic_cast<P_C_REQ_ATTACK_EVENT*>(packet->packet);

	auto mapManager = mObjectManager.lock()->GetGameObject(L"mapManager").lock();
	auto players = std::dynamic_pointer_cast<object::MapManager>(mapManager);
	auto mapDataPtr = players->GetMapData(attackPacket->mapID).lock();

	//서버에서 클라로 애니메이션 송신
	if (attackPacket->animation != 2)
	{
		P_S_NOTIFY_ATTACK_ANI_EVENT attackAniEvent;
		attackAniEvent.player = attackPacket->attackUser;
		attackAniEvent.dir = attackPacket->dir;
		attackAniEvent.animationNumber = attackPacket->animation;

		for (const auto& player : mapDataPtr->GetInMapPlayers())
		{
			auto copyPacket = attackAniEvent;
			player.second.lock()->Session->SendPacket(copyPacket);


		}
	}


	//맞는 오브젝트가 생기는 경우
	std::wstring hitName = attackPacket->hitObject;
	std::wstring hittingName = attackPacket->attackUser;
	auto hittingObject = mObjectManager.lock()->GetGameObject(hittingName);
	auto hitObject = mapDataPtr->MonsterManager.lock()->GetMonsterObject(hitName);
	if (!hitObject.expired())
	{
		if (!hitObject.lock()->GetActive())
			return;
		//hitObject.lock()->transform.position = object::Vector3(1000, 1000, 1000);
		auto hitMonster = std::dynamic_pointer_cast<object::MonsterObject>(hitObject.lock());

		hitMonster->SetHp(hitMonster->GetHp() - attackPacket->damage);

		P_S_NOTIFY_ATTACK_EVENT attackEvent;
		attackEvent.hitObject = hitMonster->Name;
		attackEvent.HP = hitMonster->GetHp();

		LOG_PRINT(LOG_LEVEL::LOG_DEBUG, L"%s to %s from attack damage %d  hp : %d\n",
			attackPacket->attackUser.c_str(), attackPacket->hitObject.c_str(), attackPacket->damage, hitMonster->GetHp());

		for (const auto& player : players->GetMapData(attackPacket->mapID).lock()->GetInMapPlayers())
		{
			auto copyPacket = attackEvent;
			player.second.lock()->Session->SendPacket(copyPacket);
		}

		if (hitMonster->GetHp() <= 0)
		{
			P_S_NOTIFY_MONSTER_MAP_UNREGISTER unregister;
			unregister.mapID = attackPacket->mapID;
			unregister.name = hitMonster->Name;

			for (const auto& player : players->GetMapData(attackPacket->mapID).lock()->GetInMapPlayers())
			{
				auto copyPacket = unregister;
				player.second.lock()->Session->SendPacket(copyPacket);
			}

			hitMonster->SetActive(false);

			
			if (db)
			{
				if (!hittingObject.expired())
				{
					auto hittingPtr = hittingObject.lock();
					auto playerPtr = std::dynamic_pointer_cast<object::PlayerObject>(hittingPtr);

					//player data update
					playerPtr->SetEXP(playerPtr->GetEXP() + hitMonster->GetExp());
					P_S_NOTIFY_CHAR_UPDATE update;
					update.exp = playerPtr->GetEXP();
					update.hp = playerPtr->GetHp();
					update.level = playerPtr->GetLevel();
					hittingPtr->Session->SendPacket(update);

					MYSQL_RES* res;
					MYSQL_ROW row;


					std::string name = std::string().assign(playerPtr->Name.begin(), playerPtr->Name.end());
					auto buffer = std::make_unique<char[]>(150);
					auto len = sprintf_s(buffer.get(), 150, "UPDATE user_character SET char_level=%d, char_exp=%d,char_hp =%d where channel_id = %d AND char_name ='%s'", playerPtr->GetLevel(), playerPtr->GetEXP(), playerPtr->GetHp(), packet->packet->Channel, name.c_str());
					int result = mysql_query(db, buffer.get());
					if (result)
					{
						LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"MYSQL UPDATE ERROR\n");
					}
				}
				mysql_free_result(res);
			}
		}
	}
}

void neo::packet::process::EventProcess::RangeAttackProcess(packet::PacketObject* packet)
{
	auto attackPacket = dynamic_cast<P_C_REQ_ATTACK_RANGE_EVENT*>(packet->packet);

	//서버에서 클라로 애니메이션 송신
	P_S_NOTIFY_ATTACK_ANI_EVENT attackAniEvent;
	attackAniEvent.player = attackPacket->attackUser;
	attackAniEvent.dir = attackPacket->dir;
	attackAniEvent.animationNumber = attackPacket->animation;

	//클라로 이펙트 실행 전송
	P_S_NOTIFY_EFFECT_EVENT effectEvent;
	effectEvent.attackUser = attackPacket->attackUser;
	effectEvent.dir = attackPacket->dir;
	effectEvent.dis = attackPacket->dis;
	effectEvent.startPos = attackPacket->startPos;


	auto mapManager = mObjectManager.lock()->GetGameObject(L"mapManager").lock();
	auto players = std::dynamic_pointer_cast<object::MapManager>(mapManager);
	auto mapDataPtr = players->GetMapData(attackPacket->mapID).lock();
	//for (auto player : players->GetInMapPlayers())
	for (const auto& player : mapDataPtr->GetInMapPlayers())
	{
		if (player.first == attackPacket->attackUser)
			continue;
		auto copyPacket = attackAniEvent;
		auto copyPacket2 = effectEvent;
		player.second.lock()->Session->SendPacket(copyPacket);
		player.second.lock()->Session->SendPacket(effectEvent);
	}
}

void neo::packet::process::EventProcess::RangeAttackHit(packet::PacketObject* packet)
{
	auto hitAttack = dynamic_cast<P_C_REQ_ATTACK_RANGE_HIT_EVENT*>(packet->packet);

	auto mapManager = mObjectManager.lock()->GetGameObject(L"mapManager").lock();
	auto players = std::dynamic_pointer_cast<object::MapManager>(mapManager);
	auto mapDataPtr = players->GetMapData(hitAttack->mapID).lock();

	std::wstring hitName = hitAttack->hitObject;
	std::wstring hittingName = hitAttack->attackUser;
	auto hittingObject = mObjectManager.lock()->GetGameObject(hittingName);
	auto hitObject = mapDataPtr->MonsterManager.lock()->GetMonsterObject(hitName);
	if (!hitObject.expired())
	{
		if (!hitObject.lock()->GetActive())
			return;
		//hitObject.lock()->transform.position = object::Vector3(1000, 1000, 1000);
		auto hitMonster = std::dynamic_pointer_cast<object::MonsterObject>(hitObject.lock());

		hitMonster->SetHp(hitMonster->GetHp() - hitAttack->damage);

		P_S_NOTIFY_ATTACK_EVENT attackEvent;
		attackEvent.hitObject = hitMonster->Name;
		attackEvent.HP = hitMonster->GetHp();

		LOG_PRINT(LOG_LEVEL::LOG_DEBUG, L"%s to %s from attack damage %d  hp : %d\n",
			hitAttack->attackUser.c_str(), hitAttack->hitObject.c_str(), hitAttack->damage, hitMonster->GetHp());

		for (const auto& player : players->GetMapData(hitAttack->mapID).lock()->GetInMapPlayers())
		{
			auto copyPacket = attackEvent;
			player.second.lock()->Session->SendPacket(copyPacket);
		}

		if (hitMonster->GetHp() <= 0)
		{
			P_S_NOTIFY_MONSTER_MAP_UNREGISTER unregister;
			unregister.mapID = hitAttack->mapID;
			unregister.name = hitMonster->Name;

			for (const auto &player : players->GetMapData(hitAttack->mapID).lock()->GetInMapPlayers())
			{
				auto copyPacket = unregister;
				player.second.lock()->Session->SendPacket(copyPacket);
			}

			hitMonster->SetActive(false);

			if (db)
			{
				if (!hittingObject.expired())
				{	
					auto hittingPtr = hittingObject.lock();
					auto playerPtr = std::dynamic_pointer_cast<object::PlayerObject>(hittingPtr);

					//player data update
					playerPtr->SetEXP(playerPtr->GetEXP() + hitMonster->GetExp());
					P_S_NOTIFY_CHAR_UPDATE update;
					update.exp = playerPtr->GetEXP();
					update.hp = playerPtr->GetHp();
					update.level = playerPtr->GetLevel();
					hittingPtr->Session->SendPacket(update);

					MYSQL_RES* res;
					MYSQL_ROW row;


					std::string name = std::string().assign(playerPtr->Name.begin(), playerPtr->Name.end());
					auto buffer = std::make_unique<char[]>(150);
					auto len = sprintf_s(buffer.get(), 150, "UPDATE user_character SET char_level=%d, char_exp=%d,char_hp =%d where channel_id = %d AND char_name ='%s'", playerPtr->GetLevel(), playerPtr->GetEXP(), playerPtr->GetHp(), packet->packet->Channel, name.c_str());
					int result = mysql_query(db, buffer.get());
					if (result)
					{
						LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"MYSQL UPDATE ERROR\n");
					}
				}
				mysql_free_result(res);
			}
		}
	}

}
