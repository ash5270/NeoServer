#pragma once
#include<string>
#include<list>
#include<network/IOCPSession.h>
namespace neo::object
{
	class ChannelInfo {
	public:
		ChannelInfo(const std::wstring& name, const int32_t number);
		~ChannelInfo();

		void RegisterChannel(network::IOCPSession* const session);
		void UnregisterChannel(network::IOCPSession* const session);
		const std::list<network::IOCPSession const*>& GetSesssion() const;
		const std::wstring GetName();
		const int32_t GetNumber()const;
	private:
		std::int32_t mChannelNumber;
		std::wstring mName;
		std::list< network::IOCPSession const*> mSessions;
	};
}