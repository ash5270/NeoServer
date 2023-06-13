#pragma once
#include"Session.h"
#include<ObjectPool.h>
namespace neo::server
{
	class SessionPool : public neo::util::system::ObjectPool<Session>
	{
	public:
		SessionPool(const size_t& size);
		~SessionPool();
	};
}
