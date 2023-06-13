#include "MapManager.h"
#include"MonsterManager.h"
#include"../ObjectManager.h"
#include<system/NeoLog.h>
void neo::object::MapManager::Update(const double& deltaTime)
{
	
}

void neo::object::MapManager::Start()
{
	mMapDatas.push_back(std::make_shared<MapData>(0));
	mMapDatas.back()->MapManager = weak_from_this();

	auto map = std::dynamic_pointer_cast<GameObject>(mMapDatas.back());
	this->Manager.lock()->RegisterObject(L"testMap", map);


	mMapDatas.push_back(std::make_shared<MapData>(1));
	mMapDatas.back()->MapManager = weak_from_this();

	auto map2 = std::dynamic_pointer_cast<GameObject>(mMapDatas.back());
	this->Manager.lock()->RegisterObject(L"test2Map", map2);


	mMapDatas.push_back(std::make_shared<MapData>(2));
	mMapDatas.back()->MapManager = weak_from_this();

	auto map3 = std::dynamic_pointer_cast<GameObject>(mMapDatas.back());
	this->Manager.lock()->RegisterObject(L"test3Map", map3);
}

void neo::object::MapManager::End()
{
	
}

void neo::object::MapManager::RegisterUser(const int16_t& mapCount, const weak_ptr<GameObject>& object)
{
	mMapDatas[mapCount]->RegisterObject(object);
	/*mPlayers.insert({ object.lock()->Name,object });*/
	auto obj = object.lock();
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"name %s\n", obj->Name.c_str());
}

void neo::object::MapManager::UnregisterUser(const int16_t& mapCount, const std::wstring& name)
{
	mMapDatas[mapCount]->UnReigsterObject(name);
	this->Manager.lock()->UnregisterObject(name);
}


