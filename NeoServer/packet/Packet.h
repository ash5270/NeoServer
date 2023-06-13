#pragma once
#include"PacketID.h"
#include<json.h>
#include<memory>
#include"../gameobject/CharPositionData.h"

//status code 
//http 코드와 비슷하다.
//200 -> OK
//201 -> Create
//204 -> NoContent
//404 -> NotFound


class TestPacket : public Packet {
public:
	PacketID GetID()
	{
		return PacketID::PI_C_REQ_LOGIN;
	}

	int32_t GetSize()
	{
		return 12;
	}

	int a = 999;
	int b = 888;
	int c = 111;

	void Serialize(OutputMemoryStream& buffer)
	{
		HeaderSerialize(buffer);
		buffer.Write(a);
		buffer.Write(b);
		buffer.Write(c);
	}

	void Deserialize(InputMemoryStream& buffer)
	{
		buffer.Read(a);
		buffer.Read(b);
		buffer.Read(c);
	}
};



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
		return sizeof(int32_t) + sizeof(int16_t) + (msg.length() - 1) * sizeof(wchar_t);
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
		return sizeof(int32_t) + sizeof(int16_t) + (ID.length() - 1) * sizeof(wchar_t);
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
		return sizeof(int32_t) + sizeof(int32_t) + sizeof(int16_t) + sizeof(wchar_t) * (msg.length() - 1);
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
		return sizeof(int32_t) + sizeof(int32_t);
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
		return json.size();
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

class P_C_UPDATE_CHAR_POSITION : public Packet
{
public:
	PacketID GetID() override
	{
		return PacketID::PI_C_UPDATE_CHAR_POSITION;
	}
	int32_t GetSize()override
	{
		return sizeof(float) + sizeof(float) +
			sizeof(float) +
			sizeof(float) +
			sizeof(float) +
			sizeof(float) +
			sizeof(int16_t) +
			sizeof(int32_t) +
			sizeof(wchar_t) * (name.length() - 1);
	}
	//member values
	std::wstring name;
	neo::object::Vector3 position;
	//velocity
	neo::object::Vector2 velocity;
	//speed
	float speed;
	int32_t animation;

	//member function
	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		//
		buffer.Write(name);
		buffer.Write(position.x);
		buffer.Write(position.y);
		buffer.Write(position.z);

		buffer.Write(velocity.x);
		buffer.Write(velocity.y);

		buffer.Write(speed);
		buffer.Write(animation);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(name);
		buffer.Read(position.x);
		buffer.Read(position.y);
		buffer.Read(position.z);

		buffer.Read(velocity.x);
		buffer.Read(velocity.y);

		buffer.Read(speed);
		buffer.Read(animation);
	}
};

class P_C_REQ_CHARACTER_REGISTER :public Packet
{
public:
	PacketID GetID()
	{
		return PacketID::PI_C_REQ_CHARACTER_REGISTER;
	}

	int32_t GetSize()
	{
		return sizeof(mapID) + sizeof(neo::object::Vector3) + sizeof(int16_t) + sizeof(wchar_t) * (name.length() - 1);
	}

	std::wstring name;
	int16_t mapID;
	neo::object::Vector3 position;

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(name);
		buffer.Write(mapID);
		buffer.Write(position.x);
		buffer.Write(position.y);
		buffer.Write(position.z);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(name);
		buffer.Read(mapID);
		buffer.Read(position.x);
		buffer.Read(position.y);
		buffer.Read(position.z);
	}
};

class P_S_RES_CHARACTER_REGISTER : public Packet
{
public:
	PacketID GetID()
	{
		return PacketID::PI_S_RES_CHARACTER_REGISTER;
	}

	int32_t GetSize()
	{
		return
			sizeof(int32_t) +
			sizeof(int16_t) + sizeof(wchar_t) * (msg.length() - 1) +
			json.size();
	}

	int32_t status;
	std::wstring msg;
	Json::Value json;

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(status);
		buffer.Write(msg);
		buffer.Write(json);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(status);
		buffer.Read(msg);
		buffer.Read(json);
	}
};

class P_C_REQ_CHARACTER_MAP_UNREGISTER :public Packet
{
public:
	PacketID GetID()
	{
		return PacketID::PI_C_REQ_CHARACTER_MAP_UNREGISTER;
	}

	int32_t GetSize()
	{
		return
			sizeof(int32_t) +
			sizeof(int16_t) + sizeof(wchar_t) * (name.length() - 1);
	}

	int16_t mapID;
	std::wstring name;

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(mapID);
		buffer.Write(name);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(mapID);
		buffer.Read(name);
	}
};


class P_S_NOTIFY_MONSTER_MAP_REGISTER :public Packet
{
public:
	PacketID GetID()
	{
		return PacketID::PI_S_NOTIFY_MONSTER_MAP_REGISTER;
	}

	int32_t GetSize()
	{
		return
			sizeof(int32_t) +
			sizeof(int16_t) + sizeof(wchar_t) * (name.length() - 1);
	}

	int16_t mapID;
	std::wstring name;

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(mapID);
		buffer.Write(name);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(mapID);
		buffer.Read(name);
	}
};


class P_S_NOTIFY_MONSTER_MAP_UNREGISTER :public Packet
{
public:
	PacketID GetID()
	{
		return PacketID::PI_S_NOTIFY_MONSTER_MAP_UNREGISTER;
	}

	int32_t GetSize()
	{
		return
			sizeof(int32_t) +
			sizeof(int16_t) + sizeof(wchar_t) * (name.length() - 1);
	}

	int16_t mapID;
	std::wstring name;

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(mapID);
		buffer.Write(name);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(mapID);
		buffer.Read(name);
	}
};

class P_S_NOTIFY_CHARACTER_MAP_UNREGISTER : public Packet
{
public:
	PacketID GetID() override
	{
		return PacketID::PI_S_NOTIFY_CHARACTER_MAP_UNREGISTER;
	}

	int32_t GetSize()
	{
		return
			sizeof(int32_t) +
			sizeof(int16_t) + sizeof(wchar_t) * (name.length() - 1);
	}

	int16_t mapID;
	std::wstring name;

	//member function
	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(mapID);
		buffer.Write(name);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(mapID);
		buffer.Read(name);
	}
};

class P_S_NOTIFY_CHARACTER_INFO :public Packet
{
public:
	PacketID GetID()
	{
		return PacketID::PI_S_NOTIFY_CHARACTER_INFO;
	}

	int32_t GetSize()
	{
		return sizeof(int16_t) + sizeof(wchar_t) * (name.length() - 1);
	}

	std::wstring name;

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(name);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(name);
	}
};

class P_S_NOTIFY_CHARACTER_POSITION : public Packet {
public:
	PacketID GetID()
	{
		return PacketID::PI_S_NOTIFY_CHAR_POSITION;
	}

	int32_t GetSize()
	{
		int32_t size = sizeof(int32_t);
		for (int i = 0; i < characters.size(); i++)
		{
			size += sizeof(characters[i].position);
			size += sizeof(characters[i].veleoctiy);
			size += sizeof(characters[i].speed);
			size += sizeof(characters[i].animation);
			size += sizeof(int16_t) + sizeof(wchar_t) * (characters[i].name.length() - 1);
		}

		return size;
	}

	std::vector<neo::object::CharPositionData> characters;

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write((int32_t)characters.size());
		for (int i = 0; i < characters.size(); i++)
		{
			buffer.Write(characters[i].position.x);
			buffer.Write(characters[i].position.y);
			buffer.Write(characters[i].position.z);
			buffer.Write(characters[i].veleoctiy.x);
			buffer.Write(characters[i].veleoctiy.y);
			buffer.Write(characters[i].speed);
			buffer.Write(characters[i].animation);
			buffer.Write(characters[i].name);
		}
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		int32_t size = 0;
		buffer.Read(size);
		for (int i = 0; i < size; i++)
		{
			neo::object::CharPositionData data;
			buffer.Read(data.position.x);
			buffer.Read(data.position.y);
			buffer.Read(data.position.z);
			buffer.Read(data.veleoctiy.x);
			buffer.Read(data.veleoctiy.y);
			buffer.Read(data.speed);
			buffer.Read(data.animation);
			buffer.Read(data.name);

			characters.push_back(data);
		}
	}
};

class P_C_NOTIFY_MAP_REGISTER : public Packet {
public:
	PacketID GetID()
	{
		return PacketID::PI_C_NOTIFY_MAP_REGISTER;
	}

	int32_t GetSize()
	{
		return sizeof(int16_t) +
			sizeof(wchar_t) * (name.length() - 1) +
			sizeof(int32_t) +
			sizeof(bool);
	}

	int32_t mapCount;
	bool isRegister;
	std::wstring name;

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(mapCount);
		buffer.Write(isRegister);
		buffer.Write(name);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(mapCount);
		buffer.Read(isRegister);
		buffer.Read(name);
	}
};




class P_C_REQ_ATTACK_EVENT : public Packet
{
public:
	PacketID GetID() override
	{
		return PacketID::PI_C_REQ_ATTACK_EVENT;
	}

	int32_t GetSize()override
	{
		return
			sizeof(int16_t) +
			sizeof(int16_t) +
			sizeof(wchar_t) * (attackUser.length() - 1) +
			sizeof(int16_t) +
			sizeof(wchar_t) * (hitObject.length() - 1) +
			sizeof(int32_t);
	}

	int16_t mapID;
	std::wstring attackUser;
	std::wstring hitObject;
	int16_t dir;
	int16_t animation;
	int32_t damage;

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(mapID);
		buffer.Write(attackUser);
		buffer.Write(hitObject);
		buffer.Write(dir);
		buffer.Write(animation);
		buffer.Write(damage);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(mapID);
		buffer.Read(attackUser);
		buffer.Read(hitObject);
		buffer.Read(dir);
		buffer.Read(animation);
		buffer.Read(damage);
	}
};

class P_S_RES_ATTACK_EVENT : public Packet
{
public:
	PacketID GetID() override
	{
		return PacketID::PI_S_RES_ATTACK_EVENT;
	}

	int32_t GetSize()override
	{
		return 0;
	}

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{

	}
};

class P_S_NOTIFY_ATTACK_ANI_EVENT : public Packet
{
public:
	PacketID GetID() override
	{
		return PacketID::PI_S_NOTIFY_ATTACK_ANI_EVENT;
	}

	int32_t GetSize()override
	{
		return sizeof(mapID) +
			sizeof(dir) + sizeof(animationNumber) +
			sizeof(int16_t) +
			sizeof(wchar_t) * (player.length() - 1);
	}

	int16_t mapID;
	int16_t dir;
	int16_t animationNumber;
	std::wstring player;

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(mapID);
		buffer.Write(dir);
		buffer.Write(animationNumber);
		buffer.Write(player);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(mapID);
		buffer.Read(dir);
		buffer.Read(animationNumber);
		buffer.Read(player);
	}
};


class P_S_NOTIFY_ATTACK_EVENT : public Packet {
public:
	PacketID GetID() override
	{
		return PacketID::PI_S_NOTIFY_ATTACK_EVENT;
	}

	int32_t GetSize()override
	{
		return sizeof(int16_t) +
			sizeof(wchar_t) * (hitObject.length() - 1) + sizeof(HP);
	}

	std::wstring hitObject;
	int32_t HP;

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(hitObject);
		buffer.Write(HP);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(hitObject);
		buffer.Read(HP);
	}
};

class P_S_NOTIFY_MONSTER_INFO : public Packet
{
public:
	PacketID GetID()
	{
		return PacketID::PI_S_NOTIFY_MONSTER_INFO;
	}

	int32_t GetSize()
	{
		return
			sizeof(int32_t) +
			sizeof(int16_t) + sizeof(wchar_t) * (msg.length() - 1) +
			json.size();
	}

	int32_t monsterCode;
	std::wstring msg;
	Json::Value json;

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(monsterCode);
		buffer.Write(msg);
		buffer.Write(json);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(monsterCode);
		buffer.Read(msg);
		buffer.Read(json);
	}
};


class P_S_NOTFIY_MONSTER_POSITION : public Packet {
public:
	PacketID GetID()
	{
		return PacketID::PI_S_NOTIFY_MONSTER_POSITION;
	}

	int32_t GetSize()
	{
		int32_t size = sizeof(int32_t);
		for (int i = 0; i < characters.size(); i++)
		{
			size += sizeof(characters[i].position);
			size += sizeof(characters[i].veleoctiy);
			size += sizeof(characters[i].speed);
			size += sizeof(characters[i].animation);
			size += sizeof(int16_t) + sizeof(wchar_t) * (characters[i].name.length() - 1);
		}

		return size;
	}

	std::vector<neo::object::CharPositionData> characters;

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write((int32_t)characters.size());
		for (int i = 0; i < characters.size(); i++)
		{
			buffer.Write(characters[i].position.x);
			buffer.Write(characters[i].position.y);
			buffer.Write(characters[i].position.z);
			buffer.Write(characters[i].veleoctiy.x);
			buffer.Write(characters[i].veleoctiy.y);
			buffer.Write(characters[i].speed);
			buffer.Write(characters[i].animation);
			buffer.Write(characters[i].name);
		}
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		int32_t size = 0;
		buffer.Read(size);
		for (int i = 0; i < size; i++)
		{
			neo::object::CharPositionData data;
			buffer.Read(data.position.x);
			buffer.Read(data.position.y);
			buffer.Read(data.position.z);
			buffer.Read(data.veleoctiy.x);
			buffer.Read(data.veleoctiy.y);
			buffer.Read(data.speed);
			buffer.Read(data.animation);
			buffer.Read(data.name);

			characters.push_back(data);
		}
	}
};

class P_S_NOTIFY_CHAR_UPDATE : public Packet
{
public:
	PacketID GetID()
	{
		return PacketID::PI_S_NOTIFY_CHAR_UDPATE;
	}

	int32_t GetSize()
	{
		int32_t size = sizeof(hp) + sizeof(exp) + sizeof(level);
		return size;
	}

	int32_t hp;
	int32_t exp;
	int32_t level;

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(hp);
		buffer.Write(exp);
		buffer.Write(level);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(hp);
		buffer.Read(exp);
		buffer.Read(level);
	}
};



class P_S_NOTIFY_EFFECT_EVENT : public Packet
{
public:
	PacketID GetID()
	{
		return PacketID::PI_S_NOTIFY_EFFECT_EVENT;
	}

	int32_t GetSize()
	{
		int32_t size = sizeof(neo::object::Vector3) + sizeof(int16_t) +
			sizeof(wchar_t) * (attackUser.length() - 1) + sizeof(float) + sizeof(int16_t);
		return size;
	}

	neo::object::Vector3 startPos;
	std::wstring attackUser;
	float dis;
	int16_t dir;

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(startPos.x);
		buffer.Write(startPos.y);
		buffer.Write(startPos.z);
		buffer.Write(attackUser);
		buffer.Write(dis);
		buffer.Write(dir);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(startPos.x);
		buffer.Read(startPos.y);
		buffer.Read(startPos.z);
		buffer.Read(attackUser);
		buffer.Read(dis);
		buffer.Read(dir);
	}
};


class P_C_REQ_ATTACK_RANGE_EVENT :public Packet
{
public:
	PacketID GetID()
	{
		return PacketID::PI_C_REQ_ATTACK_RANGE_EVENT;
	}

	int32_t GetSize()
	{
		int32_t size = sizeof(neo::object::Vector3) + sizeof(int16_t) +
			sizeof(wchar_t) * (attackUser.length() - 1) + sizeof(float) + sizeof(int32_t) + sizeof(int32_t)
			+ sizeof(int16_t) + sizeof(int16_t);
		return size;
	}

	neo::object::Vector3 startPos;
	int16_t mapID;
	int16_t dir;
	std::wstring attackUser;
	float dis;
	int32_t damage;
	int32_t animation;

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(startPos.x);
		buffer.Write(startPos.y);
		buffer.Write(startPos.z);
		buffer.Write(mapID);
		buffer.Write(dir);
		buffer.Write(attackUser);
		buffer.Write(dis);
		buffer.Write(damage);
		buffer.Write(animation);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(startPos.x);
		buffer.Read(startPos.y);
		buffer.Read(startPos.z);
		buffer.Read(mapID);
		buffer.Read(dir);
		buffer.Read(attackUser);
		buffer.Read(dis);
		buffer.Read(damage);
		buffer.Read(animation);
	}
};

class P_C_REQ_ATTACK_RANGE_HIT_EVENT :public Packet
{
public:
	PacketID GetID()
	{
		return PacketID::PI_C_REQ_ATTACK_RANGE_HIT_EVENT;
	}

	int32_t GetSize()
	{
		int32_t size = sizeof(int16_t) + sizeof(int32_t)
			+ sizeof(int16_t) + sizeof(wchar_t) * (attackUser.length() - 1)
			+ sizeof(int16_t) + sizeof(wchar_t) * (hitObject.length() - 1);
		return size;
	}

	int16_t mapID;
	std::wstring attackUser;
	std::wstring hitObject;
	int32_t damage;

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(mapID);
		buffer.Write(attackUser);
		buffer.Write(hitObject);
		buffer.Write(damage);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(mapID);
		buffer.Read(attackUser);
		buffer.Read(hitObject);
		buffer.Read(damage);
	}
};

class P_C_REQ_LOGOUT : public Packet {
public:
	PacketID GetID()
	{
		return PacketID::PI_C_REQ_LOGOUT;
	}

	int32_t GetSize()
	{
		int32_t size = sizeof(int16_t) + 
			 sizeof(int16_t) + sizeof(wchar_t) * (user.length() - 1);
		return size;
	}

	int16_t mapID;
	std::wstring user;
	

	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(mapID);
		buffer.Write(user);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(mapID);
		buffer.Read(user);
	}
};

class P_S_RES_LOGOUT : public Packet {
public:
	PacketID GetID()
	{
		return PacketID::PI_S_RES_LOGOUT;
	}

	int32_t GetSize()
	{
		int32_t size = 
			sizeof(int16_t) + sizeof(wchar_t) * (user.length() - 1);
		return size;
	}

	std::wstring user;


	void Serialize(OutputMemoryStream& buffer) override
	{
		HeaderSerialize(buffer);
		buffer.Write(user);
	}

	void Deserialize(InputMemoryStream& buffer) override
	{
		buffer.Read(user);
	}
};