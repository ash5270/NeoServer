#pragma once
#include<atomic>

namespace neo::util::system
{
	class SpinLock {
	public:
		SpinLock() : lock(ATOMIC_FLAG_INIT) {

		}

		void Lock()
		{
			//이 명령어 아래 있는 것을 여기 위로 명령어가 재배치 되는걸 금지함
			//플래그를 true로 설정 초기 플래그 값을 반환
			while (lock.test_and_set(std::memory_order_acquire))
			{

			}
		}

		bool TryLock()
		{
			return !lock.test_and_set(std::memory_order_acquire);
		}
		
		void UnLock()
		{
			//이전 명령어가 다음 명령어 가지 않게 막음
			lock.clear(std::memory_order_release);
		}

		void Clear()
		{
			UnLock();
		}

	private:
		std::atomic_flag lock;
	};

	template<typename T>
	class SpinLockGraud
	{
	public:
		SpinLockGraud(T& lock) :mLock(lock)
		{
			mLock.Lock();
		}

		~SpinLockGraud()
		{
			mLock.UnLock();
		}
	private	:
		T& mLock;
	};
}