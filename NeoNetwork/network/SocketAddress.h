#pragma once
#include<stdint.h>
#include "SocketCommon.h"


namespace neo::network
{
	class SocketAddress 
	{
	public:
		SocketAddress(uint32_t address, uint16_t port)
		{
			memset(&mSockAddr, 0, sizeof(sockaddr));
			GetSockAddrIn()->sin_family = AF_INET;
			GetSockAddrIn()->sin_addr.S_un.S_addr = htonl(address);
			GetSockAddrIn()->sin_port = htons(port);
		}

		SocketAddress(const sockaddr& sockaddr)
		{
			memcpy(&mSockAddr, &sockaddr, sizeof(sockaddr));
		}

		size_t GetSize() const { return sizeof(mSockAddr); }
		
		const sockaddr* GetSockAddr() const
		{
			return &mSockAddr;
		}


	private:
		friend class TCPSocket;
		sockaddr_in* GetSockAddrIn()
		{
			return reinterpret_cast<sockaddr_in*>(&mSockAddr);
		}
		sockaddr mSockAddr;
	};
}