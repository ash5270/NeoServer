#include "IOCPSocket.h"

#include <process.h>

#include "IOCPSession.h"

UINT WINAPI CallIOThread(LPVOID ptr)
{
	neo::network::IOCPSocket* overlapped = static_cast<neo::network::IOCPSocket*>(ptr);
	overlapped->WorkingThread();
	return 0;
}

neo::network::IOCPSocket::IOCPSocket()
{
	//thread 실행 여부
	mIOThreadCheck = true;
}

neo::network::IOCPSocket::~IOCPSocket()
{
	mIOThreadCheck = false;
	WSACleanup();
}

bool neo::network::IOCPSocket::CreateIOThread(size_t numThread)
{
	UINT32 threadId;
	mIOThreadHandle = std::make_unique<HANDLE[]>(numThread);

	for (int i = 0; i < numThread; i++)
	{
		//쓰레드 생성 //_beginthreadex ex아닌 함수
		mIOThreadHandle[i] = reinterpret_cast<HANDLE*>(_beginthreadex(nullptr, 0,
			&CallIOThread, this, CREATE_SUSPENDED, &threadId));
		if (mIOThreadHandle[i] == nullptr)
		{
			//error 처리
			throw std::exception("IOThread Create Error...");
		}
		//thread start
		ResumeThread(mIOThreadHandle[i]);
	}

	return true;
}

void neo::network::IOCPSocket::WorkingThread()
{
	//에러 체크용 bool 변수 
	BOOL result;

	DWORD transferSize;
	//completionKey
	IOCPSession* session;
	//overlapped Data
	IOCPData* iocpData;
	DWORD dwFlags = 0;

	while (mIOThreadCheck)
	{
		result = GetQueuedCompletionStatus(mIOCPHandle, &transferSize, reinterpret_cast<PULONG_PTR>(&session),
			reinterpret_cast<LPOVERLAPPED*>(&iocpData), INFINITE);

		if (!result || transferSize == 0)
		{
			//socket error
			//socket close
			session->OnClose();
			closesocket(iocpData->GetSocket());
			delete iocpData;
			continue;
		}
		//IO_Type 별 데이터 분류
		switch (iocpData->GetIOType())
		{
		case IO_TYPE::IO_SEND:
			session->OnSend(transferSize);
			break;
		case IO_TYPE::IO_READ:
			session->OnRecv(transferSize);
			break;
		case IO_TYPE::IO_ACCEPT:
			OnAccept(iocpData->GetSocket());
			break;
		default:
			//
			wprintf_s(L"IO_TYPE ERROR \n");
			break;
		}
	}

}

void neo::network::IOCPSocket::CloseSocket()
{

}

bool neo::network::IOCPSocket::WSAInit()
{
	int result = 0;
	WSADATA wsaData;
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		//error
		return false;
	}
	return true;
}

void neo::network::IOCPSocket::OnAccept(const size_t& transferSize)
{

}
