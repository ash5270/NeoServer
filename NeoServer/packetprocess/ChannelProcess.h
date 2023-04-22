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

		virtual void Process(packet::PacketObejct* packet) override;
	private:
		void ChannelInfoRespone(const packet::PacketObejct* packet);
		void ChannelRegister(const packet::PacketObejct* packet);
	};

}