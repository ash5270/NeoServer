#pragma once
#include "../packet/PacketObject.h"
namespace neo::packet::process
{
	class BasePacketProcess
	{
	public:
		virtual ~BasePacketProcess() = default;
		virtual void Process(const neo::PacketObjPtr&) = 0;
	};
}
