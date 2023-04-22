#include "LogicThread.h"


neo::system::LogicThread::LogicThread()
{
	mPacketQueue = std::make_shared< util::system::LockFreeQueue<packet::PacketObejct*>>();
	mPacketProcess = std::make_unique<packet::PacketProcess>();
}

neo::system::LogicThread::~LogicThread()
{
}

void neo::system::LogicThread::Start()
{
	mLoopThread = std::thread(&LogicThread::Update, this);
	mIsLoop = true;
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"Logic thread Start...\n");
}

void neo::system::LogicThread::Stop()
{
	mIsLoop = false;
	if(mLoopThread.joinable())
		mLoopThread.join();
}

void neo::system::LogicThread::Update()
{
    mIsLoop = true;
    if (mPacketQueue==nullptr)
        return;
    //update
    while (mIsLoop)
    {
        if (mPacketQueue->Empty())
        {
            //������� ��� ���ؽ�Ʈ ����ġ
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            continue;
        }
        else
        {
            //��Ŷ���μ��� ����
            packet::PacketObejct* packetObj;
            mPacketQueue->Dequeue(packetObj);
            mPacketProcess->GetFunc(packetObj->packet->GetID())(packetObj);
            delete packetObj;
        }
    }
}
 