#include "MapData.h"
#include"../ObjectManager.h"
neo::object::MapData::MapData(const int32_t& mapID) :mMapID(mapID)
{

}

neo::object::MapData::~MapData()
{
}

void neo::object::MapData::Update(const double& deltaTime)
{
	//모든 플레이어 위치 정보 저장 후
	P_S_NOTIFY_CHARACTER_POSITION position_packet;
	//한번에 보내기 
	for (auto const& player : mPlayers)
	{
		auto object = player.second.lock();
		CharPositionData data;
		data.position = object->transform.position;
		data.veleoctiy = object->transform.veloctiy;
		data.speed = 10;
		data.name = object->Name;
		data.animation = object->Animation;
		position_packet.characters.push_back(std::move(data));
	}

	for (auto const& player : mPlayers)
	{
		auto copyPacket = position_packet;
		auto object = player.second.lock();
		if (!object->Session->IsConnect())
			mCloseSession.push(object);
		else
			object->Session->SendPacket(copyPacket);
	}

	for (int i = 0; i < mCloseSession.size(); i++)
	{
		auto ptr = mCloseSession.top().lock();
		UnReigsterObject(ptr->Name);

		P_S_NOTIFY_CHARACTER_MAP_UNREGISTER unregister;
		unregister.name = ptr->Name;
		unregister.mapID = this->mMapID;

		mCloseSession.pop();

		for (auto const& player : mPlayers)
		{
			auto copyPacket = unregister;
			auto object = player.second.lock();
			if (object->Session->IsConnect())
				object->Session->SendPacket(copyPacket);
		}

	}
}

void neo::object::MapData::Start()
{
	//몬스터 매니저 등록
	//몬스터 관리 
	std::shared_ptr<GameObject> monsterManager 
		= std::make_shared<neo::object::MonsterManager>(shared_from_this());
	
	auto objectManagerPtr = this->Manager.lock();
	std::wstring objectName = L"monsterManager";
	objectName.append(to_wstring(mMapID));
	objectManagerPtr->RegisterObject(objectName, monsterManager);

	auto monsterManagerPtr 
		= std::dynamic_pointer_cast<object::MonsterManager>(monsterManager);
	MonsterManager = monsterManagerPtr;
	if(mMapID!=0)
		monsterManagerPtr->CreateMonster(mMapID);
}

void neo::object::MapData::End()
{

}

void neo::object::MapData::RegisterObject(const weak_ptr<GameObject>& object)
{
	mPlayers.insert({ object.lock()->Name, object });
}

void neo::object::MapData::UnReigsterObject(const std::wstring& name)
{
	LOG_PRINT(LOG_LEVEL::LOG_DEBUG, L"unrgister Object : %s\n", name.c_str());
	mPlayers.erase(name);
}

Json::Value neo::object::MapData::GetJsonGameObject(const std::wstring& name)
{
	Json::Value root;
	Json::Value array;
	for (auto const& object : mPlayers)
	{
		if (name == object.first)
			continue;
		Json::Value obj;
		obj["name"] = std::string().assign(object.second.lock()->Name.begin(), object.second.lock()->Name.end());
		array.append(std::move(obj));
	}
	root["array"] = array;
	return root;
}

std::unordered_map<std::wstring, weak_ptr<neo::object::GameObject>>& neo::object::MapData::GetInMapPlayers()
{
	return mPlayers;
}
