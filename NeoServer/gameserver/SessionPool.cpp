#include "SessionPool.h"

neo::server::SessionPool::SessionPool(const size_t& size) : neo::util::system::ObjectPool<neo::server::Session>(size)
{
}

neo::server::SessionPool::~SessionPool()
{
}
