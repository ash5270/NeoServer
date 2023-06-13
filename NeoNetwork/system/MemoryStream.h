//½ºÆ®¸² Base
#pragma once	
#include<iostream>
#include<memory>
#include<json.h>


#include"Buffer.h"
#include"ByteSwaper.h"
namespace neo::system
{
	class MemoryStream
	{
	public:
		MemoryStream(char* buffer, const int32_t capacity) : mBuffer(buffer), mCapacity(capacity), mHead(0) 
		{

		}

		MemoryStream(const Buffer& buffer) :mBuffer(buffer.GetDataPtr()), mCapacity(buffer.GetCapacity()), mHead(0)
		{

		}

		virtual ~MemoryStream() {
			mBuffer = nullptr;
		}

	protected:
		char* mBuffer;
		int32_t mHead;
		size_t mCapacity;

	public:
		char* GetStreamPtr() const { return mBuffer; }
		int32_t GetLength() const { return mHead; }
		size_t GetCapacity() const { return mCapacity; }
		void SetOffset(const int32_t& offset) { mHead = offset; }
	
	};
}

