//span 
//C# 처럼 메모리를 할당받아서 사용하는 것이 아닌
//이미 할당된 버퍼를 잠깐 일정 기간동안 사용하는 것

#include<iostream>
#include"NeoLog.h"

namespace neo::system
{
	class Span {
	public:
		Span(char* buffer, const size_t& size) :mHead(0), mBuffer(buffer), mCapactiy(size)
		{

		}

		~Span()
		{

		}

	public:
		inline char* GetBuffer() const
		{
			//참조하고 있는 데이터가 null일경우
			if (mBuffer == nullptr)
			{
				LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"span pointer is null\n");
				return nullptr;
			}
			else
			{
				return mBuffer;
			}
		}
		//어디까지 사용했는지 반환
		inline int32_t GetOffset() const
		{
			return mHead;
		}
		//clear
		inline void Clear()
		{
			mHead = 0;
		}
		//크기 반환
		size_t GetCapactiy() const
		{
			return mCapactiy;
		}

		inline void SetOffset(const size_t& size)
		{
			mHead = size;
		}

	private:
		char* mBuffer;
		size_t mHead;
		const size_t mCapactiy;
	};
}