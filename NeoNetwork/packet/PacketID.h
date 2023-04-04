#pragma once
#include<stdint.h>
#include<string>
#include "../system/Buffer.h"
#include"../system/InputMemoryStream.h"
#include"../system/OutputMemoryStream.h"
using namespace neo::system;


enum PacketID : int32_t
{
    PI_C_REQ_LOGIN = 2000,
    PI_S_RES_LOGIN = 2001,
};

struct PacketHeader {
    int32_t id;
    int32_t size;
};

class Packet
{
public:
    PacketHeader header;
    virtual PacketID GetID() = 0;
    virtual int32_t GetSize() = 0;
    virtual void Serialize(OutputMemoryStream& buffer) = 0;
    virtual void Deserialize(InputMemoryStream& buffer) = 0;
};