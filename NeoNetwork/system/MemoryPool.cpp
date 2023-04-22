#include "MemoryPool.h"

neo::system::MemoryPool::MemoryPool(const size_t& blockSize, const size_t& blokcCount) : mBlockSize(blockSize), mBlockCount(blokcCount)
{
	mTotalSize = blokcCount * blockSize;
	mBuffer = new char[mTotalSize];

	auto curPos = mBuffer;
	for (size_t i = 0; i < mBlockCount; i++)
	{
		mQueue.Enqueue(curPos);
		curPos += mBlockSize;
	}
}

neo::system::MemoryPool::~MemoryPool()
{
	delete[] mBuffer;
	mBuffer = nullptr;
}

char* const neo::system::MemoryPool::Aollocate()
{
	char* buffer;
	if (mQueue.Dequeue(buffer))
	{
		return buffer;
	}
	else
		return nullptr;
}

void neo::system::MemoryPool::Free(char* const buffer)
{
	mQueue.Enqueue(buffer);
}

size_t neo::system::MemoryPool::GetBlockSize() const
{
	return mBlockSize;
}

size_t neo::system::MemoryPool::GetBlockCount() const
{
	return mBlockCount;
}

