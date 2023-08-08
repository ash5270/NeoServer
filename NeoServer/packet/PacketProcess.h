//packet 처리 함수 관리 클래스

#pragma once
#include<unordered_map>
#include<functional>
#include"NeoPacket.h"
#include"Packet.pb.h"
using namespace neo::util::system;

namespace neo::packet
{
    class PacketProcess
    {
    public:
        PacketProcess();
        ~PacketProcess();

        void RegisterProcess(const PacketID& packetID, std::function<void(const neo::PacketObjPtr&)>func);
        void UnRegisterProcee(const PacketID& pakcetID);

        std::function<void(std::unique_ptr<PacketObj>)> GetFunc(const PacketID& packetID)
        {
            if (mProcessMap.find(packetID) != mProcessMap.end())
            {
                return mProcessMap[packetID];
            }
            else
                return nullptr;
        }
    private:
        //packet process 패킷 처리용
        std::unordered_map <PacketID, std::function<void(std::unique_ptr<PacketObj>)>> mProcessMap;
        //loop condition
        bool mIsLoop = false;
    };
}
