#include "IOCPData.h"

neo::network::IOCPData::IOCPData(IO_TYPE type):mIOType(type)
{
	InitIOCPData();
}

neo::network::IOCPData::IOCPData(IO_TYPE type, const SOCKET& socket): mIOType(type),mSocket(socket)
{
	InitIOCPData();
}

neo::network::IOCPData::IOCPData(IO_TYPE type, const SOCKET& socket, const size_t& bufLen):mIOType(type), mSocket(socket)
{
	InitIOCPData();
}

void neo::network::IOCPData::InitIOCPData()
{
	memset(&mOverlapped, 0, sizeof(OVERLAPPED));
}

neo::network::IOCPData::~IOCPData()
{

}

 WSABUF* neo::network::IOCPData::GetWSABuf() 
{
	return &mWSABuf;
}

neo::network::IO_TYPE neo::network::IOCPData::GetIOType() const
{
	return mIOType;
}

SOCKET neo::network::IOCPData::GetSocket() const
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

char* neo::network::IOCPData::GetBuffer() const
{
	return mWSABuf.buf;
}

void neo::network::IOCPData::SetBuffer( char* buffer, const size_t& size)
{
	mWSABuf.buf = buffer;
	mWSABuf.len = size;
}

void neo::network::IOCPData::SetBuffer(const WSABUF& buf)
{
	mWSABuf = buf;
}
