//InputStream 
#pragma once

#include "MemoryStream.h"
#include <string>
namespace neo::system
{
	class InputMemoryStream : public MemoryStream
	{
	public:
		InputMemoryStream(char* buffer,const uint32_t& capacity);
		~InputMemoryStream();

		uint32_t GetRemainingSize()const { return mCapacity - mHead; }
		void Read(void* outData, uint32_t inByteSize);

	public:
		//8
		void Read(char& outData) {
			Read(&outData, sizeof(outData));
		}
		//16
		void Read(int16_t& outData) {
			Read(&outData, sizeof(outData));
		}
		//32
		void Read(int32_t& outData) {
			Read(&outData, sizeof(outData));
		}
		//64
		void Read(int64_t& outData) {
			Read(&outData, sizeof(outData));
		}

		//32
		void Read(float& outData) {
			Read(&outData, sizeof(outData));
		}
		//64
		void Read(double& outData) {
			Read(&outData, sizeof(outData));
		}

		void Read(std::wstring& outData)
		{
			//string ±Ê¿Ã πﬁæ∆ø»
			int16_t length = 0;
			Read(&length, sizeof(length));
			//string ±Ê¿Ã ¥√∑¡µ“
			outData.reserve(length + 1);
			Read(&outData[0], length);
		}
	};
}