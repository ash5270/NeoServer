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

    while (mIsLoop)
    {
        if (GetPopQueue().empty())
        {
            std::this_thread::sleep_for(std::chrono::microseconds(20));
            continue;
        }
        else
        {
            auto packetObject = std::move(GetPopQueue().front());
            GetPopQueue().pop();
            neo::PacketID packetId = packetObject->packet.header.packetID;
            mPacketProcess->GetFunc(packetId)(std::move(packetObject));
        }
    }
}
