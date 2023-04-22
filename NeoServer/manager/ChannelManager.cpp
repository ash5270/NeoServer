#include "ChannelManager.h"
#include<string>

void neo::object::ChannelManager::Init()
{
	mChannelCount = 1;
}

void neo::object::ChannelManager::Stop()
{

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

void neo::object::ChannelManager::AddUser(const int& channelID, neo::network::IOCPSession* session)
{
	int id = channelID - 1; 
	mChannels[id]->RegisterChannel(session);
}

void neo::object::ChannelManager::RemoveUser(const int& channelID, neo::network::IOCPSession* session)
{
	int id = channelID - 1;
	mChannels[id]->UnregisterChannel(session);
}


