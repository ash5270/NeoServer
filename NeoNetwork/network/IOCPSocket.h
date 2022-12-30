﻿//소켓 관련 클래스

#pragma once

#include "SocketCommon.h"
#include <memory>

namespace network
{
	class IOCPSocket
	{
	public:
		IOCPSocket();
		~IOCPSocket();

		//IO Thread 생성 함수
		bool CreateIOThread(size_t numThread);
		//IO 스레드 시작 함수
		void WorkingThread();

		//소켓 닫기
		void CloseSocket();

		//IOCPSocket 사용을 위한 WSA 초기화
		bool WSAInit();

	private:
		//IOCP Handle
		HANDLE mIOCPHandle;

		//thread 관리 핸들
		std::unique_ptr<HANDLE[]> mIOThreadHandle;

		//thread 동작 유뮤
		bool mIOThreadCheck;
	};

}

