//스트림 Base
#pragma once	
#include<iostream>
#include<memory>

#include"Buffer.h"
#include"ByteSwaper.h"
namespace neo::system
{
	class MemoryStream 
	{
	public:
		MemoryStream():mHead(0),mCapacity(0),mBuffer(nullptr), mIsCopyBuffer(false){
			ReallocMemory(1024);
		}
		
		MemoryStream(char* buffer, const int32_t capacity) : mBuffer(buffer), mCapacity(capacity),mHead(0) {
			mIsCopyBuffer = true;
		}

		MemoryStream(const Buffer& buffer):mBuffer(buffer.GetDataPtr()),mCapacity(buffer.GetCapacity()),mHead(0)
		{
			mIsCopyBuffer = true;
		}

		virtual ~MemoryStream() {
			if (!mIsCopyBuffer)
			{
				std::free(mBuffer);
				mBuffer = nullptr;
			}
		}

	protected:
		char* mBuffer;
		int32_t mHead;
		int32_t mCapacity;
		
		bool mIsCopyBuffer;
	public:
		char* GetStreamPtr() const { return mBuffer; }
		int32_t GetLength() const { return mHead; }
		int32_t GetCapacity() const { return mCapacity; }

	protected:
		void ReallocMemory(int32_t inNewCapacity) {
			mBuffer = static_cast<char*>
				(std::realloc(mBuffer,
					inNewCapacity));
			//실패시 처리
			if (mBuffer == nullptr)
			{
				wprintf_s(L"ERROR : Buffer realloc memory error\n");
				std::free(mBuffer);
			}
			//메모리 크기를 변경
			mCapacity = inNewCapacity;
		}
	};
}

