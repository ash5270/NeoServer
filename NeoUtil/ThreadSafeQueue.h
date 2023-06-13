#pragma once
#include<Windows.h>

#include<queue>
#include<deque>

namespace neo::util::system
{
	template<class T>
	class ThreadSafeQueue
	{
	public:
		ThreadSafeQueue() {
			::InitializeCriticalSection(&mCs);
			mQueue.resize(2097152);
		}

		~ThreadSafeQueue() {
			for (int i = 0; i < mQueue.size(); i++)
			{
				mQueue.pop_front();
			}

			::DeleteCriticalSection(&mCs);
		}

	public:
		void push(const T& data)
		{
			::EnterCriticalSection(&mCs);
			mQueue.push_back(data);
			::LeaveCriticalSection(&mCs);
		}


		T& popFront()
		{
			::EnterCriticalSection(&mCs);
			auto data = mQueue.front();
			mQueue.pop_front();
			::LeaveCriticalSection(&mCs);
			return data;
		}

		void FrontPush(const T& data)
		{
			::EnterCriticalSection(&mCs);
			mQueue.push_front(data);
			::LeaveCriticalSection(&mCs);
		}

		T& front()
		{
			::EnterCriticalSection(&mCs);
			auto data = mQueue.front();
			::LeaveCriticalSection(&mCs);
			return data;
		}

		void Pop()
		{
			::EnterCriticalSection(&mCs);
			mQueue.pop_front();
			::LeaveCriticalSection(&mCs);
		}

		bool TryPopFront(T& data)
		{
			bool result = false;
			::EnterCriticalSection(&mCs);
			if (mQueue.empty())
			{
				result = false;
			}
			else {
				data= mQueue.front();
				mQueue.pop_front();
				result = true;
			}
			::LeaveCriticalSection(&mCs);
			return result;
		}

		size_t Size()
		{
			return mQueue.size();
		}

		bool IsEmpty()
		{
			::EnterCriticalSection(&mCs);
			auto result = mQueue.empty();
			::LeaveCriticalSection(&mCs);
			return result;
		}


	private:
		std::deque<T> mQueue;
		::CRITICAL_SECTION mCs;
	};
}

