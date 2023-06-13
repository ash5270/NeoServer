#pragma once
#include<stdint.h>
#include<iostream>
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

		SocketAddress(const std::string& address, uint16_t port)
		{  
			memset(&mSockAddr, 0, sizeof(sockaddr));
			GetSockAddrIn()->sin_family = AF_INET;
			inet_pton(AF_INET, address.c_str(),
				&GetSockAddrIn()->sin_addr.S_un.S_addr);
			GetSockAddrIn()->sin_port = htons(port);
		}
		
		SocketAddress(const SocketAddress& socketAddress)
		{
			memcpy(&mSockAddr, &socketAddress.mSockAddr, sizeof(sockaddr));
		}

		SocketAddress(SocketAddress&& socketAddress) noexcept
		{
			memcpy(&mSockAddr, &socketAddress.mSockAddr, sizeof(sockaddr));
			memset(&socketAddress.mSockAddr, 0, sizeof(sockaddr));
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