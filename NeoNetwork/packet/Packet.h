#pragma once
#include"PacketID.h"

class P_C_REQ_LOGIN : public Packet
{
public:
    PacketID GetID() override
    {
        return PI_C_REQ_LOGIN;
    }
    int32_t GetSize()override
    {
        return sizeof(int16_t) + user_id.length() + sizeof(int8_t);
    }
    //member values
    std::wstring user_id;
    int32_t uuid;


    //member function
    void Serialize(OutputMemoryStream& buffer) override
    {
        buffer.Write(user_id);
        buffer.Write(uuid);

    }

    void Deserialize(InputMemoryStream& buffer) override
    {
        buffer.Read(user_id);
        buffer.Read(uuid);
    }
};

class P_S_RES_LOGIN : public Packet
{
public:
    PacketID GetID() override
    {
        return PI_S_RES_LOGIN;
    }
    int32_t GetSize()override
    {
        return sizeof(int32_t);
    }
    //member values
    int32_t status_code;


    //member function
    void Serialize(OutputMemoryStream& buffer) override
    {
        buffer.Write((int)GetID());
        buffer.Write(GetSize());
        buffer.Write(status_code);
    }

    void Deserialize(InputMemoryStream& buffer) override
    {
        buffer.Read(status_code);

    }
};

