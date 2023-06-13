//채널 관리용 매니저
//
#pragma once
#include<vector>
#include<memory>
#include<json.h>

#include<Singleton.h>
//
#include"ChannelInfo.h"
#include"../gameserver/Session.h"

namespace neo::object
{
	class ChannelManager : public Singleton< ChannelManager>
	{
	public:
		void Init();
		void Stop();
		void AddChannel(const std::wstring& name);
		Json::Value GetJsonAllChannelInfo();
		std::vector<std::unique_ptr<ChannelInfo>>& GetChannels();

		void AddUser(const int& channelID, neo::server::Session* session);
		void RemoveUser(const int& channelID, neo::server::Session* session);
	private:
		const static int CHANNEL_SIZE = 2;
		std::vector<std::unique_ptr<ChannelInfo>> mChannels;
		int mChannelCount;
		CRITICAL_SECTION mCs;
	};
}