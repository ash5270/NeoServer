//IOCP에 사용할 Overlapped 데이터

#pragma once
#include "SocketCommon.h"

namespace neo::network
{
	enum class IO_TYPE
	{
		IO_READ = 0,
		IO_SEND = 1,
		IO_ACCEPT=2,
	};

	class IOCPData
	{
	public:
		IOCPData(IO_TYPE type);
		IOCPData(IO_TYPE type,const SOCKET& socket);
		//아직 구현 안함
		IOCPData(IO_TYPE type,const SOCKET& socket,const size_t& bufLen);
		~IOCPData();

		WSABUF* GetWASBuf();
		IO_TYPE GetIOType();
		SOCKET GetSocket();
		//OVERLAPPED 데이터 가져오기
		OVERLAPPED* GetOverlapped();
		//socket 할당 : 세션 풀링을 위해
		void SetSocket(SOCKET socket);
		char* GetBuffer();

	private:
		OVERLAPPED mOverlapped;
		IO_TYPE mIOType;
		//임시 버퍼
		char mBuffer[1024];
		WSABUF mWSABuf;
		SOCKET mSocket;
	};

}


