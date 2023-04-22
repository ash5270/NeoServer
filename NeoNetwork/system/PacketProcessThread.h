//로직이 필요없는 패킷들을 위한 처리 쓰레드 

#pragma once
#include"LockFreeQueue.h"
#include"../system/NeoLog.h"
#include"../packet/PacketProcess.h"
#include"../packet/PacketObject.h"

namespace neo::system
{
	class PacketProcessThread
	{
	public:
		PacketProcessThread();
		~PacketProcessThread();

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
