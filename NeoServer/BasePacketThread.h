#pragma once

#include"packet/NeoPacketFactory.h"
#include"packet/NeoPacket.h"
#include"packet/PacketProcess.h"
#include<Windows.h>
#include<queue>
#include<vector>
#include<atomic>

//TODO : lock free queue를 빼고 큐를 두개 두어서 처리할 예정
//atomic을 사용하여 swap 이 되었을 경우
//다시 queue 가지고 와서 사용함


namespace neo::system
{
	class BasePacketThread
	{
	public:
		BasePacketThread(const int& id): mThreadId(id),mInsertQueue(0),mPopQueue(1)
		{
			//mPacketQueue = std::make_shared< util::system::LockFreeQueue<packet::PacketObject*>>();
			mPacketProcess = std::make_unique<packet::PacketProcess>();
			::InitializeCriticalSection(&mCritical);
			mSwapCheck.store(false);
		}

		virtual ~BasePacketThread()
		{
			mPacketProcess.release();
			::DeleteCriticalSection(&mCritical);
		}


		virtual void Start()=0;
		virtual void Stop()=0;
		virtual packet::PacketProcess* GetPacketProcess() = 0;

		//두개의 queue를 스왑해줌. 데이터를 꺼내는 쪽에서만 사용
		void SwapPacketQueue()
		{
			//queue 두개 쓸 예정이기 떄문에 
			//두개를 바꿔서 사용할 예정 
			//그럴려면 어떻게 해야할까?
			::EnterCriticalSection(&mCritical);
			//여기서 스왑하기 
			bool check = false;
			mSwapCheck.compare_exchange_strong(check, true);

			mInsertQueue = mInsertQueue ? 0 : 1;
			mPopQueue = mPopQueue ? 0 : 1;

			bool swap = true;
			mSwapCheck.compare_exchange_strong(check, false);
			::LeaveCriticalSection(&mCritical);
		}

		std::queue<PacketObjPtr>& GetInsertQueue()
		{
			while (mSwapCheck.load())
			{
				std::this_thread::sleep_for(std::chrono::microseconds(20));
			}
			return  mPacketQueues[mInsertQueue];
		}

		std::queue<PacketObjPtr>& GetPopQueue()
		{
			while (mSwapCheck.load())
			{
				std::this_thread::sleep_for(std::chrono::microseconds(20));
			}
			return mPacketQueues[mPopQueue];
		}


	protected:
		virtual void ThreadUpdate() = 0;
	protected:
		std::unique_ptr<packet::PacketProcess> mPacketProcess;
		//std::shared_ptr<util::system::LockFreeQueue<packet::PacketObject*>> mPacketQueue;

		//loop condition
		bool mIsLoop = false;
		std::thread mLoopThread;
		int mThreadId=0;

	private:
		//queue 두개를 만들어서 사용할 생각->
		int mInsertQueue;
		int mPopQueue;
		std::queue<PacketObjPtr> mPacketQueues[2];

		//atomic
		std::atomic<bool> mSwapCheck;
		//critical section 
		::CRITICAL_SECTION mCritical;
	};

}


