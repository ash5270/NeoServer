//스트림 Base
#pragma once	
#include<iostream>
#include<memory>
namespace neo::system
{
	class MemoryStream 
	{
	public:
		MemoryStream():mHead(0),mCapacity(0),mBuffer(nullptr) {
			ReallocMemory(1024);
		}
		
		MemoryStream(char* buffer, const uint32_t capacity) : mBuffer(buffer), mCapacity(capacity),mHead(0) {

		}

		virtual ~MemoryStream() {
			std::free(mBuffer);
		}

	protected:
		char* mBuffer;
		uint32_t mHead;
		uint32_t mCapacity;
		
	public:
		char* GetStreamPtr() const { return mBuffer; }
		uint32_t GetLength() const { return mHead; }
		uint32_t GetCapacity() const { return mCapacity; }

	protected:
		void ReallocMemory(uint32_t inNewCapacity) {
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

