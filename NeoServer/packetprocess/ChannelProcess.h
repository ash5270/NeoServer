#pragma once
#include"BasePacketProcess.h"
#include"../manager/ChannelManager.h"

namespace neo::packet::process
{
	class ChannelProcess : public BasePacketProcess
	{
	public:
		ChannelProcess();
		~ChannelProcess();
		virtual void Process(packet::PacketObject* packet) override;
	private:
		void ChannelInfoRespone(const packet::PacketObject* packet);
		void ChannelRegister(const packet::PacketObject* packet);

		
	};

}