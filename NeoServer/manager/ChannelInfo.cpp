#include "ChannelInfo.h"
neo::object::ChannelInfo::ChannelInfo(const std::wstring& name, const int32_t number)
	:mName(name),mChannelNumber(number)
{
	
}

neo::object::ChannelInfo::~ChannelInfo()
{

}

void neo::object::ChannelInfo::RegisterChannel(unique_ptr<server::Session>& sesion)
{
	mSessions.push_back(session);
	//ssion->AddRef();
}

void neo::object::ChannelInfo::UnregisterChannel(unique_ptr<server::Session>& session)
{
	mSessions.remove(session);
	//session->RemoveRef();
}

 std::list<neo::server::Session *>& neo::object::ChannelInfo::GetSesssion() 
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

