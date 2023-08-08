#pragma once

#include"packet/NeoPacketFactory.h"
#include"packet/NeoPacket.h"
#include"packet/PacketProcess.h"
#include<Windows.h>
#include<queue>
#include<vector>
#include<atomic>

//TODO : lock free queue�� ���� ť�� �ΰ� �ξ ó���� ����
//atomic�� ����Ͽ� swap �� �Ǿ��� ���
//�ٽ� queue ������ �ͼ� �����


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

		//�ΰ��� queue�� ��������. �����͸� ������ �ʿ����� ���
		void SwapPacketQueue()
		{
			//queue �ΰ� �� �����̱� ������ 
			//�ΰ��� �ٲ㼭 ����� ���� 
			//�׷����� ��� �ؾ��ұ�?
			::EnterCriticalSection(&mCritical);
			//���⼭ �����ϱ� 
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
		//queue �ΰ��� ���� ����� ����->
		int mInsertQueue;
		int mPopQueue;
		std::queue<PacketObjPtr> mPacketQueues[2];

		//atomic
		std::atomic<bool> mSwapCheck;
		//critical section 
		::CRITICAL_SECTION mCritical;
	};

}


