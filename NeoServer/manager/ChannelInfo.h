#pragma once
#include<string>
#include<list>

#include "../gameserver/Session.h"

namespace neo::object
{
	class ChannelInfo {
	public:
		ChannelInfo(const std::wstring& name, const int32_t number);
		~ChannelInfo();

		void RegisterChannel(server::Session* const session);
		void UnregisterChannel(server::Session* const session);
		std::list<server::Session *>& GetSesssion() ;
		const std::wstring GetName();
		const int32_t GetNumber()const;
	private:
		std::int32_t mChannelNumber;
		std::wstring mName;
		std::list<server::Session *> mSessions;
	};
}