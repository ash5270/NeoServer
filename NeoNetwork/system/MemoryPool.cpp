#include "MemoryPool.h"

neo::system::MemoryPool::MemoryPool(const size_t& blockSize, const size_t& blokcCount) : mBlockSize(blockSize), mBlockCount(mBlockCount)
{
	mTotalSize = mBlockCount * mBlockSize;
	mBuffer = new char8_t[mTotalSize];

	auto curPos = mBuffer;
	for (size_t i = 0; i < mBlockCount; i++)
	{
		mQueue.Dequeue(curPos);
		curPos += mBlockSize;
	}
}

neo::system::MemoryPool::~MemoryPool()
{
	delete[] mBuffer;
	mBuffer = nullptr;
}

char8_t* const neo::system::MemoryPool::Aollocate()
{
	char8_t* buffer;
	if (mQueue.Dequeue(buffer))
	{
		return buffer;
	}
	else
		return nullptr;
}

void neo::system::MemoryPool::Free( char8_t* const buffer)
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

