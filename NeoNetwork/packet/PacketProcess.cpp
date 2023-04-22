#include "PacketProcess.h"
#include <thread>

neo::packet::PacketProcess::PacketProcess()
{
}

neo::packet::PacketProcess::~PacketProcess()
{
    
}

void neo::packet::PacketProcess::RegisterProcess(const PacketID& packetID, std::function<void(packet::PacketObejct*)>func)
{
    //패킷 프로세스 추가 
    if(mProcessMap.find(packetID)==mProcessMap.end())
    {
        mProcessMap.insert({ packetID,func });
    }
}

void neo::packet::PacketProcess::UnRegisterProcee(const PacketID& packetID)
{
    //패킷 프로세스 제거
    if(mProcessMap.find(packetID)==mProcessMap.end())
    {
        return;
    }
    mProcessMap.erase(packetID);
}



