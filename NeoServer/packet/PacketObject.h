#pragma once
#include"PacketID.h"
#include"../gameserver/Session.h"
namespace neo::packet {
	struct PacketObject {
		Packet* packet;
		server::Session* session;
	};
}