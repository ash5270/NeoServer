//기본적인 스트림 클래스
//버퍼랑 비슷한 개념이라고 보자

#pragma once
#include <memory>
namespace neo::system
{
	class Buffer
	{
	public:
		Buffer(const size_t& size) : mCapacity(size),mHead(0)
		{
			mBuf = std::make_unique<char[]>(size);
		}

		~Buffer()
		{
			
		}

		Buffer(const Buffer& buffer) : mCapacity(buffer.GetCapacity())
		{
			//memory copy
			mBuf = std::make_unique<char[]>(buffer.GetCapacity());
			memcpy_s(mBuf.get(),
				mCapacity,
				buffer.GetDataPtr(),
				buffer.GetCapacity());
			mHead = buffer.mHead;
		}

	public:
		inline char* GetDataPtr() const
		{
			return mBuf.get();
		}

		inline size_t GetCapacity() const
		{
			return mCapacity;
		}

		inline size_t GetSize() const
		{
			return mCapacity - mHead;
		}

		inline size_t GetOffset() const
		{
			return mHead;
		}

		inline void SetOffset(const size_t& offset)
		{
			mHead = offset;
		}

	private:
		//크기
		size_t mCapacity;
		//buf
		std::unique_ptr<char[]> mBuf;
		//offset
		size_t mHead = 0;
	};
}
