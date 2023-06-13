#pragma once
#include<Windows.h>
#include<vector>
#include<memory>
namespace neo::util::system {
	
	template<class T>
	class ObjectPool
	{
	public:
		ObjectPool(const size_t& size)
		{
			for (int i = 0; i < size; i++)
				mFreeObjects.push_back(std::make_shared<T>());
			::InitializeCriticalSection(&mSection);
		}

		~ObjectPool()
		{
			for (auto ptr : mFreeObjects)
			{
				ptr.reset();
			}
		}

		std::shared_ptr<T> GetSharedObject()
		{
			::EnterCriticalSection(&mSection);
			if (mFreeObjects.empty())
			{

			}
			
		    auto obj = mFreeObjects.back();
			mFreeObjects.pop_back();
			::LeaveCriticalSection(&mSection);
			return obj;
		}

		void ReleaseObject(std::shared_ptr<T> object)
		{
			::EnterCriticalSection(&mSection);
			mFreeObjects.push_back(object);
				::LeaveCriticalSection(&mSection);
		}

	private:
		std::vector<std::shared_ptr<T>> mFreeObjects;
		CRITICAL_SECTION mSection;
	};
}