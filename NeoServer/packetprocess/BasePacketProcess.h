#pragma once
#include<packet/PacketObject.h>
namespace neo::packet::process
{
	class BasePacketProcess
	{
	public:
		virtual void Process(packet::PacketObejct* packet) = 0;
	};
}
