#pragma once
#include"PacketID.h"
#include<json.h>
#include<memory>

 
//status code 
//http 코드와 비슷하다.
//200 -> OK
//201 -> Create
//204 -> NoContent
//404 -> NotFound


class P_C_REQ_LOGIN : public Packet
{
public:
	PacketID GetID() override
	{
		return PacketID::PI_C_REQ_LOGIN;
	}
	int32_t GetSize()override
	{
		return id.length() * sizeof(wchar_t);
	}
	//member values
	std::wstring id;
	std::unique_ptr<char> uuid = std::make_unique<char>(16);

	//member function
	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(id);
		buffer.Write(uuid.get(), 16);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(id);
		buffer.Read(uuid.get(), 16);
	}
};

class P_S_RES_LOGIN : public Packet
{
public:
	PacketID GetID() override
	{
		return PacketID::PI_S_RES_LOGIN;
	}
	int32_t GetSize()override
	{
		return sizeof(int32_t) + sizeof(int16_t) + msg.length() * sizeof(wchar_t);
	}
	//member values
	int32_t status = 0;
	std::wstring msg;


	//member function
	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(status);
		buffer.Write(msg);

	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(status);
		buffer.Read(msg);
	}
};

class P_C_REQ_CHANNEL_REGISTER : public Packet
{
public:
	PacketID GetID() override
	{
		return PacketID::PI_C_REQ_CHANNEL_REGISTER;
	}
	int32_t GetSize()override
	{
		return sizeof(int32_t)+sizeof(int16_t)+ ID.length() * sizeof(wchar_t);
	}
	//member values
	int32_t channelID;
	std::wstring ID;


	//member function
	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(channelID);
		buffer.Write(ID);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(channelID);
		buffer.Read(ID);
	}
};

class P_S_RES_CHANNEL_REGISTER : public Packet
{
public:
	PacketID GetID() override
	{
		return PacketID::PI_S_RES_CHANNEL_REGISTER;
	}
	int32_t GetSize()override
	{
		return sizeof(int32_t) + sizeof(int32_t) + sizeof(int16_t)+ sizeof(wchar_t) * msg.length();
	}
	//member values
	int32_t status;
	int32_t channelID;
	std::wstring msg;

	//member function
	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(status);
		buffer.Write(channelID);
		buffer.Write(msg);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(status);
		buffer.Read(channelID);
		buffer.Read(msg);
	}
};

class P_C_NOTIFY_CHANNEL_UNREGISTER : public Packet
{
public:
	PacketID GetID() override
	{
		return PacketID::PI_C_NOTIFY_CHANNEL_UNREGISTER;
	}
	int32_t GetSize()override
	{
		return sizeof(int32_t) + sizeof(int32_t) + sizeof(int16_t);
	}
	//member values
	int32_t status;
	int32_t channelID;

	//member function
	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(status);
		buffer.Write(channelID);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(status);
		buffer.Read(channelID);
	}
};

class P_S_NOTIFY_CHANNEL_INFO : public Packet
{
public:
	PacketID GetID() override
	{
		return PacketID::PI_S_NOTIFY_CHANNEL_INFO;
	}
	int32_t GetSize()override
	{
		return sizeof(int32_t) + sizeof(int32_t);
	}
	//member values
	Json::Value json;

	//member function
	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		//
		buffer.Write(json);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(json);
	}
};



