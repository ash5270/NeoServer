#pragma once
#include"BasePacketProcess.h"
#include"../packet/PacketObject.h"
#include"../ObjectManager.h"

namespace neo::packet::process
{
	class UtilProcess : public BasePacketProcess
	{
	public:
		UtilProcess();
		~UtilProcess();
		virtual void Process(packet::PacketObject* packet) override;
	private:
		void Ping(packet::PacketObject* packet);
	};
}