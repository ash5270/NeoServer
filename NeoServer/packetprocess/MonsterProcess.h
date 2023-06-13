#pragma once
#include"BasePacketProcess.h"
#include"../packet/PacketObject.h"
#include"../ObjectManager.h"

namespace neo::packet::process
{
	class MonsterProcess : public BasePacketProcess
	{
	public:
		MonsterProcess(const int32_t& channel, std::weak_ptr<server::ObjectManager> objectManager);
		~MonsterProcess();

		void Process(packet::PacketObject* packet) override;
	private:
	};


}

