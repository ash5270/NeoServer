#include "IOCPData.h"

neo::network::IOCPData::IOCPData(IO_TYPE type):mIOType(type)
{
	
}

WSABUF* neo::network::IOCPData::GetWASBuf()
{
	return &mWSABuf;
}

neo::network::IO_TYPE neo::network::IOCPData::GetIOType()
{
	return mIOType;
}

SOCKET neo::network::IOCPData::GetSocket()
{
	return mSocket;
}

OVERLAPPED* neo::network::IOCPData::GetOverlapped()
{
	return &mOverlapped;
}

void neo::network::IOCPData::SetSocket(SOCKET socket)
{
	mSocket = socket;
}

char* neo::network::IOCPData::GetBuffer()
{
	return mBuffer;
}
