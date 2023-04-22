#include "ChannelInfo.h"
neo::object::ChannelInfo::ChannelInfo(const std::wstring& name, const int32_t number)
	:mName(name),mChannelNumber(number)
{
	
}

neo::object::ChannelInfo::~ChannelInfo()
{

}

void neo::object::ChannelInfo::RegisterChannel(neo::network::IOCPSession* const session)
{
	mSessions.push_back(session);
	session->AddRef();
}

void neo::object::ChannelInfo::UnregisterChannel(neo::network::IOCPSession* const session)
{
	mSessions.remove(session);
	session->RemoveRef();
}

const std::list<neo::network::IOCPSession const*>& neo::object::ChannelInfo::GetSesssion() const
{
	return mSessions;
}

const std::wstring neo::object::ChannelInfo::GetName()
{
	return mName;
}

const int32_t neo::object::ChannelInfo::GetNumber() const
{
	return mChannelNumber;
}

