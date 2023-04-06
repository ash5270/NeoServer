#pragma once
#include "LockFreeQueue.h";

namespace neo::system {
	class MemoryPool
	{
	public:
		//block size,block count;
		MemoryPool(const size_t& blockSize, const size_t& blokcCount);
		~MemoryPool();
	public:
		//할당
		char8_t* const Aollocate();
		//해제
		void Free(char8_t* const buffer);
		size_t GetBlockSize() const;
		size_t GetBlockCount() const;
	private:
		neo::util::system::LockFreeQueue<char8_t*> mQueue;
		char8_t* mBuffer = nullptr;
		size_t mBlockSize = 0;
		size_t mBlockCount = 0;
		size_t mTotalSize = 0;
	};
}

