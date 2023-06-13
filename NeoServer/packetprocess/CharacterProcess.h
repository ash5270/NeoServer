#pragma once
#include"BasePacketProcess.h"
#include"../packet/PacketObject.h"
#include"../ObjectManager.h"
#include"../gameobject/MapManager.h"


namespace neo::packet::process
{
	class CharacterProcess : public BasePacketProcess
	{
	public:
		CharacterProcess(const int32_t& channel ,std::weak_ptr<server::ObjectManager> objectManager);
		~CharacterProcess();
		virtual void Process(packet::PacketObject* packet) override;
	private:
		void UpdatePosition(const packet::PacketObject* packet);
		void UpdatePlayerMap(const packet::PacketObject* packet);
		void RegisterUser(const packet::PacketObject* packet);

		void MapRegisterUser(const packet::PacketObject* packet);
		void MapUnregisterUser(const packet::PacketObject* packet);

		std::weak_ptr<server::ObjectManager> mObjectManager;
		std::weak_ptr<object::MapManager> mMapmanager;
		//current channel
		int32_t mChannel;
	};
}