#include "UtilProcess.h"

neo::packet::process::UtilProcess::UtilProcess()
{
}

neo::packet::process::UtilProcess::~UtilProcess()
{
}

void neo::packet::process::UtilProcess::Process(packet::PacketObject* packet)
{
	if (packet->packet->GetID() == PacketID::PI_C_NOTIFY_PING)
	{
		Ping(packet);
	}
	delete packet->packet;
}

void neo::packet::process::UtilProcess::Ping(packet::PacketObject* packet)
{
	P_S_NOTIFY_PING ping;
	auto pingPacket = dynamic_cast<P_C_NOTIFY_PING*>(packet->packet);
	LOG_PRINT(LOG_LEVEL::LOG_DEBUG, L"Ping %ld ,\n", pingPacket->time);

	ping.time = pingPacket->time;
	packet->session->SendPacket(ping);
}
