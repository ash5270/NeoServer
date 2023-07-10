#pragma once
#include"BasePacketProcess.h"
#include"../packet/PacketObject.h"
#include"../ObjectManager.h"
#include"../ObjectManager.h"
#include"../database/DataBaseManager.h"
namespace neo::packet::process
{
	class EventProcess : public BasePacketProcess
	{
	public:
		EventProcess(const int32_t& channel, std::weak_ptr<server::ObjectManager> objectManager);
		~EventProcess();

		virtual void Process(packet::PacketObject* packet) override;
	private:
		void AttackProcess(packet::PacketObject* packet);
		void RangeAttackProcess(packet::PacketObject* packet);
		void RangeAttackHit(packet::PacketObject* packet);
		std::weak_ptr<server::ObjectManager> mObjectManager;
		int32_t mChannel;
		MYSQL* db;
	};
}