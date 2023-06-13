#include "PacketProcessThread.h"
neo::system::PacketProcessThread::PacketProcessThread(const int& id):BasePacketThread(id)
{
	
}

neo::system::PacketProcessThread::~PacketProcessThread()
{

}

void neo::system::PacketProcessThread::Start()
{
	mLoopThread = std::thread(&PacketProcessThread::ThreadUpdate, this);
	mIsLoop = true;
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"Packet Process Thread Start...   id : %d\n",mThreadId);
}

void neo::system::PacketProcessThread::Stop()
{
	mIsLoop = false;
	if (mLoopThread.joinable())
		mLoopThread.join();
}

void neo::system::PacketProcessThread::ThreadUpdate()
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
            packet::PacketObject* packetObj;
            mPacketQueue->Dequeue(packetObj);
            mPacketProcess->GetFunc(packetObj->packet->GetID())
                (packetObj);
            delete packetObj;
        }
    }
}
