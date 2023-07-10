#pragma once
#include<json.hpp>
#include<list>
#include<unordered_map>
#include<stack>

#include"GameObject.h"
#include"MapData.h"

namespace neo::object
{
	class MapManager :public GameObject , public std::enable_shared_from_this<MapManager>
	{
	public:
		void Update(const double& deltaTime) override;
		void Start() override;
		void End() override;

		void RegisterUser(const int16_t& mapCount, const weak_ptr<GameObject>& object);
		void UnregisterUser(const int16_t& mapCount, const std::wstring& name);

		std::weak_ptr<MapData> GetMapData(const int32_t& mapID)
		{
			return mMapDatas[mapID];
		}
	private:
		//map datas
		std::vector<std::shared_ptr<MapData>> mMapDatas;
	};
}


