//기본적인 스트림 클래스
//버퍼랑 비슷한 개념이라고 보자

#pragma once
#include <memory>
namespace neo::system
{
	class Buffer
	{
	public:
		Buffer(const size_t& size) : mCapacity(size)
		{
			mBuf = std::make_unique<char[]>(size);
		}

		~Buffer()
		{
			mBuf.release();
		}

		Buffer(const Buffer& buffer) : mCapacity(buffer.GetCapacity())
		{
			//memory copy
			mBuf = std::make_unique<char[]>(buffer.GetCapacity());
			memcpy_s(mBuf.get(),
				mCapacity,
				buffer.GetDataPtr(),
				buffer.GetCapacity());
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

	private:
		size_t mCapacity;
		std::unique_ptr<char[]> mBuf;
	};
}
