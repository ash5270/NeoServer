#pragma once
#include<json.hpp>
#include<stack>
#include<unordered_map>
#include<memory>
#include"MonsterManager.h"
#include"GameObject.h"

namespace neo::object
{
	class MapManager;
	class MapData : public GameObject, public std::enable_shared_from_this<MapData>
	{
	public:
		MapData(const int32_t& mapID);
		~MapData();
		
		void Update(const double& deltaTime) override;
		void Start() override;
		void End() override;

	public:
		nlohmann::json GetJsonGameObject(const std::wstring& name);
		std::unordered_map<std::wstring, weak_ptr<GameObject>>& GetInMapPlayers();
		
		void RegisterObject(const weak_ptr<GameObject>& object);
		void UnReigsterObject(const std::wstring& name);

		std::weak_ptr<MapManager> MapManager;
		std::weak_ptr<MonsterManager> MonsterManager;

	private:
		//connect session
		std::unordered_map<std::wstring, weak_ptr<GameObject>> mPlayers;
		//close session
		std::stack<weak_ptr<GameObject>> mCloseSession;
		int32_t mMapID;


	};
}


