#include "IOCPData.h"

neo::network::IOCPData::IOCPData(IO_TYPE type):mIOType(type)
{
	mBuffer = std::make_unique<Buffer>(1024);
	memset(&mOverlapped, 0, sizeof(OVERLAPPED));

}

neo::network::IOCPData::IOCPData(IO_TYPE type, const SOCKET& socket): mIOType(type),mSocket(socket)
{
	mBuffer = std::make_unique<Buffer>(1024);
	memset(&mOverlapped, 0, sizeof(OVERLAPPED));
}

neo::network::IOCPData::IOCPData(IO_TYPE type, const SOCKET& socket, const size_t& bufLen):mIOType(type), mSocket(socket)
{
	mBuffer = std::make_unique<Buffer>(1024);
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

neo::Buffer* neo::network::IOCPData::GetBuffer() const
{
	return mBuffer.get();
}

