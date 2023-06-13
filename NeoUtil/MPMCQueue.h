#pragma once
#include<atomic>
#include<assert.h>

namespace neo::util::system
{
	template<typename T>
	class MPMCQueue
	{
	public:
		MPMCQueue(const size_t& bufferSize) :mBuffer(new cell_t[bufferSize]), mBufferMask(bufferSize - 1)
		{
			assert((bufferSize >= 2) && ((bufferSize & (bufferSize - 1)) == 0));
			for (size_t i = 0; i != bufferSize; i += 1)
			{
				mBuffer[i].Sequence.store(i, std::memory_order_relaxed);
			}

			mEnqueuePos.store(0, std::memory_order_relaxed);
			mDequeuePos.store(0, std::memory_order_relaxed);
		}
		~MPMCQueue()
		{
			delete[] mBuffer;
		}

		bool enqueue(T const& data)
		{
			cell_t *cell;
			size_t pos = mEnqueuePos.load(std::memory_order_relaxed);
			for (;;)
			{
				cell = &mBuffer[pos & mBufferMask];
				size_t seq = cell->Sequence.load(std::memory_order_acquire);
				intptr_t dif = (intptr_t)seq - (intptr_t)pos;
				if (dif == 0)
				{
					if (mEnqueuePos.compare_exchange_weak
					(pos, pos + 1, std::memory_order_relaxed))
						break;
				}
				else if (dif < 0)
					return false;
				else
					pos = mEnqueuePos.load(std::memory_order_relaxed);
			}
			cell->Data = data;
			cell->Sequence.store(pos + 1, std::memory_order_release);
			return true;
		}

		bool dequeue(T& data)
		{
			cell_t* cell;
			size_t pos = mDequeuePos.load(std::memory_order_relaxed);
			for (;;)
			{
				cell = &mBuffer[pos & mBufferMask];
				size_t seq = cell->Sequence.load(std::memory_order_acquire);
				intptr_t dif = (intptr_t)seq - (intptr_t)(pos + 1);
				if (dif == 0)
				{
					if (mDequeuePos.compare_exchange_weak(
						pos, pos + 1, std::memory_order_relaxed))
						break;
				}
				else if (dif < 0)
				{
					return false;
				}
				else
					pos = mDequeuePos.load(std::memory_order_relaxed);
			}

			data = cell->Data;
			cell->Sequence.store(pos + mBufferMask + 1, std::memory_order_release);
			return true;
		}

		bool IsEmpty()
		{
			if (mEnqueuePos.load(std::memory_order_relaxed) == mDequeuePos.load(std::memory_order_relaxed))
				return true;
			else
				false;
		}

	private:
		struct cell_t
		{
			std::atomic<size_t> Sequence;
			T Data;
		};

		static size_t const cacheline_size = 64;
		typedef char cacheline_pad_t[cacheline_size];
		cacheline_pad_t mPad0;
		cell_t* const mBuffer;
		size_t const mBufferMask;
		cacheline_pad_t mPad1;
		std::atomic<size_t> mEnqueuePos;
		cacheline_pad_t mPad2;
		std::atomic<size_t> mDequeuePos;
		cacheline_pad_t mPad3;

		//
		MPMCQueue(MPMCQueue const&);
		void operator=(MPMCQueue const&);
	};
}