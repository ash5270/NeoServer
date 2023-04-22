#include "PacketProcessThread.h"
neo::system::PacketProcessThread::PacketProcessThread()
{
	mPacketQueue = std::make_shared< util::system::LockFreeQueue<packet::PacketObejct*>>();
	mPacketProcess = std::make_unique<packet::PacketProcess>();
}

neo::system::PacketProcessThread::~PacketProcessThread()
{
}

void neo::system::PacketProcessThread::Start()
{
	mLoopThread = std::thread(&PacketProcessThread::Update, this);
	mIsLoop = true;
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"Packet Process Thread Start...\n");
}

void neo::system::PacketProcessThread::Stop()
{
	mIsLoop = false;
	if (mLoopThread.joinable())
		mLoopThread.join();
}

void neo::system::PacketProcessThread::Update()
{
    mIsLoop = true;
    if (mPacketQueue == nullptr)
        return;

    while (mIsLoop)
    {
        if (mPacketQueue->Empty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            continue;
        }
        else
        {
            packet::PacketObejct* packetObj;
            mPacketQueue->Dequeue(packetObj);
            mPacketProcess->GetFunc(packetObj->packet->GetID())(packetObj);
            delete packetObj;
        }
    }
}
