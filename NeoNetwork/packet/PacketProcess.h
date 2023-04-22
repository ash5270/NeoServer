//packet 처리 함수 관리 클래스

#pragma once
#include<unordered_map>
#include<functional>
#include"PacketID.h"
#include"LockFreeQueue.h"
#include "PacketObject.h"

using namespace neo::util::system;

namespace neo::packet
{
    class PacketProcess
    {
    public:
        PacketProcess();
        ~PacketProcess();

        void RegisterProcess(const PacketID& packetID, std::function<void(PacketObejct* )>func);
        void UnRegisterProcee(const PacketID& pakcetID);
        std::function<void(PacketObejct*)> GetFunc(const PacketID& packetID)
        {
            return mProcessMap[packetID];
        }
    private:
        //packet process 패킷 처리용
        std::unordered_map <PacketID, std::function<void(PacketObejct*)>> mProcessMap;
        //loop condition
        bool mIsLoop = false;
    };
}
