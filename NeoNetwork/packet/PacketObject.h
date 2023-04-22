#pragma once
#include"PacketID.h"
#include"../network/IOCPSession.h"
namespace neo::packet {
	struct PacketObejct {
		Packet* packet;
		network::IOCPSession* session;
	};
}