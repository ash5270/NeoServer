#include "LogicThread.h"
#include <sysinfoapi.h>
#include <chrono>

neo::system::LogicThread::LogicThread(const int& id) : BasePacketThread(id)
{
	mObjectManager = std::make_shared<neo::server::ObjectManager>();

	mTime = std::make_unique<Time>();
}

neo::system::LogicThread::~LogicThread()
{

}

void neo::system::LogicThread::Start()
{
	mIsLoop = true;
	mLoopThread = std::thread(&LogicThread::ThreadUpdate, this);
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"Logic thread Start...  id : %d\n", mThreadId);
}

void neo::system::LogicThread::Stop()
{
	mIsLoop = false;
	if (mLoopThread.joinable())
		mLoopThread.join();
}
 
void neo::system::LogicThread::ThreadUpdate()
{
	mIsLoop = true;
	if (mPacketQueue == nullptr)
	{
		LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"Packet Queue Error...\n");
		return;
	}

	const int TICK_PER_SECOND = 25;
	const int SKIP_TICKS = 1000 / TICK_PER_SECOND;
	const int MAX_FRAMESKIP = 5;

	ULONGLONG next_game_tick = GetTickCount64();
	int loops = 0;
	float interpolation;
	float time = 0;

	//update
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"Update Start...\n");
	while (mIsLoop)
	{
		//패킷프로세스 시작
		
		while (!mPacketQueue->Empty())
		{
			packet::PacketObject* packetObj;
			mPacketQueue->Dequeue(packetObj);
			mPacketProcess->GetFunc(
				packetObj->packet->GetID())(packetObj);
			delete packetObj;
		}
		
		loops = 0;
		while (GetTickCount64() > next_game_tick && loops < MAX_FRAMESKIP)
		{
			mTime->Update();
			if(mObjectManager->GetSize()>0)
				ComponentUpdate(mTime->GetDeltaTime());
			next_game_tick += SKIP_TICKS;
			loops++;
		}

		interpolation = float(GetTickCount64() + SKIP_TICKS - next_game_tick) / float(SKIP_TICKS);
	}

	//end
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"Object End...\n");
	mObjectManager->ObjectEnd();
}

void neo::system::LogicThread::ComponentUpdate(const double& deltaTime)
{
	mObjectManager->ObjectUpdate(deltaTime);
}
