//buffer
//통신간 데이터를 주고 받을 데이터 공간


#pragma once
#include <memory>
namespace neo
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
