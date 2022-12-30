#include "IOCPSocket.h"
#include <process.h>

UINT WINAPI CallIOThread(LPVOID ptr)
{
	network::IOCPSocket* overlapped = static_cast<network::IOCPSocket*>(ptr);
	overlapped->WorkingThread();
	return 0;
}

network::IOCPSocket::IOCPSocket()
{
	mIOThreadCheck = true;
}

network::IOCPSocket::~IOCPSocket()
{
	mIOThreadCheck = false;
	WSACleanup();
}

bool network::IOCPSocket::CreateIOThread(size_t numThread)
{
	UINT32 threadId;
	mIOThreadHandle = std::make_unique<HANDLE[]>(numThread);

	for(int i=0; i<numThread; i++)
	{
		mIOThreadHandle[i] = reinterpret_cast<HANDLE*>(_beginthreadex(nullptr,0, 
			&CallIOThread, this, CREATE_SUSPENDED, &threadId));
		if(mIOThreadHandle[i]==nullptr)
		{
			
		}
	}

	return true;
}

void network::IOCPSocket::WorkingThread()
{
	//에러 체크용 bool 변수 
	bool result;
	 
}

void network::IOCPSocket::CloseSocket()
{
}

bool network::IOCPSocket::WSAInit()
{
	return false;
}
