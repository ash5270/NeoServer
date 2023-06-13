#pragma once

#include"LockFreeQueue.h"
#include"../packet/PacketProcess.h"
#include"../packet/PacketObject.h"

namespace neo::system
{
	class BasePacketThread
	{
	public:
		BasePacketThread(const int& id): mThreadId(id)
		{
			mPacketQueue = std::make_shared< util::system::LockFreeQueue<packet::PacketObject*>>();
			mPacketProcess = std::make_unique<packet::PacketProcess>();
		}

		virtual ~BasePacketThread()
		{
			mPacketProcess.release();
		}

		virtual void Start()=0;
		virtual void Stop()=0;
		virtual std::shared_ptr<util::system::LockFreeQueue<packet::PacketObject*>> GetPacketQueue() = 0;
		virtual packet::PacketProcess* GetPacketProcess() = 0;

	protected:
		virtual void ThreadUpdate() = 0;
	protected:
		std::unique_ptr<packet::PacketProcess> mPacketProcess;
		std::shared_ptr<util::system::LockFreeQueue<packet::PacketObject*>> mPacketQueue;

		//loop condition
		bool mIsLoop = false;
		std::thread mLoopThread;

		int mThreadId;
	};

}


