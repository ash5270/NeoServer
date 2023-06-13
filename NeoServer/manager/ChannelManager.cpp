#include "ChannelManager.h"
#include "../database/DatabaseCommon.h"
#include<string>

void neo::object::ChannelManager::Init()
{
	mChannelCount = 1;
	::InitializeCriticalSection(&mCs);

	MYSQL* conn;
	conn = mysql_init(0);
	conn = mysql_real_connect(conn, MYSQL_IP.c_str(), MYSQL_ID.c_str(), MYSQL_PASSWORD.c_str(),
		MYSQL_DB.c_str(), MYSQL_PORT, NULL, 0);

	if (conn)
	{
		std::string Query = "SELECT channel_name FROM channel_info ";

		MYSQL_RES* res;
		MYSQL_ROW row;
		const char* q = Query.c_str();
		int result = mysql_query(conn, q);
		if (!result)
		{
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res))
			{
				string str = row[0];
				wstring channel_name;
				channel_name.assign(str.begin(), str.end());
				ChannelManager::GetInstance().AddChannel(channel_name);
				LOG_PRINT(LOG_LEVEL::LOG_INFO, L"Add channel namd : %s\n", channel_name.c_str());
			}
		}
	}
}

void neo::object::ChannelManager::Stop()
{
	::DeleteCriticalSection(&mCs);
}

void neo::object::ChannelManager::AddChannel(const std::wstring& name)
{
	mChannels.push_back(make_unique<neo::object::ChannelInfo>(name, mChannelCount++));
}

Json::Value neo::object::ChannelManager::GetJsonAllChannelInfo()
{
	Json::Value root; 
	Json::Value array;
	for (int i = 0; i < mChannels.size(); i++)
	{
		Json::Value channel;
		auto name = mChannels[i]->GetName();
		channel["name"] = std::string().assign(name.begin(), name.end());
		channel["userCount"] = mChannels[i]->GetSesssion().size();
		channel["id"] = mChannels[i]->GetNumber();
		array.append(std::move(channel));
	}
	root["array"] = array;
	return root;
}

std::vector<std::unique_ptr<neo::object::ChannelInfo>>& neo::object::ChannelManager::GetChannels()
{
	return mChannels;
}

void neo::object::ChannelManager::AddUser(const int& channelID, neo::server::Session* session)
{
	::EnterCriticalSection(&mCs);
	int id = channelID - 1; 
	session->SetChannelId(id);
	mChannels[id]->RegisterChannel(session);
	::LeaveCriticalSection(&mCs);
}

void neo::object::ChannelManager::RemoveUser(const int& channelID, neo::server::Session* session)
{
	::EnterCriticalSection(&mCs);
	int id = channelID;
	mChannels[id]->UnregisterChannel(session);
	::LeaveCriticalSection(&mCs);
}


