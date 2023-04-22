//로직을 처리하기 위한 쓰레드
//일정 틱을 가지고 패킷을 처리함

#pragma once
#include"LockFreeQueue.h"
#include"../system/NeoLog.h"
#include"../packet/PacketProcess.h"
#include"../packet/PacketObject.h"
namespace neo::system {
	class LogicThread
	{
	public:
		LogicThread();
		~LogicThread();

	public:
		void Start();
		void Stop();
		std::shared_ptr<util::system::LockFreeQueue<packet::PacketObejct*>> GetPacketQueue()
		{
			return mPacketQueue;
		}

		packet::PacketProcess* GetPacketProcess()
		{
			return mPacketProcess.get();
		}
	private:
		void Update();

	private:
		std::unique_ptr<packet::PacketProcess> mPacketProcess;
		std::shared_ptr<util::system::LockFreeQueue<packet::PacketObejct*>> mPacketQueue;

		//loop condition
		bool mIsLoop = false;
		std::thread mLoopThread;
	};
}
