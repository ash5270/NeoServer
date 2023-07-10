//InputStream 
#pragma once

#include "MemoryStream.h"
#include <vector>
#include <algorithm>
#include <string>
namespace neo::system
{
	class InputMemoryStream : public MemoryStream
	{
	public:
		InputMemoryStream(char* buffer,const uint32_t& capacity);
		InputMemoryStream(const Buffer& buffer);
		~InputMemoryStream();

		uint32_t GetRemainingSize()const { return mCapacity - mHead; }
		void Read(void* outData, uint32_t inByteSize);

	public:
		void Read(bool& outData)
		{
			Read(&outData, sizeof(outData));
		}

		//8 
		void Read(char& outData) {
			Read(&outData, sizeof(outData));
		}
		//16
		void Read(int16_t& outData) {
			if (std::endian::native == std::endian::little)
			{
				Read(&outData, sizeof(outData));
				int16_t result = ByteSwap(outData);
				outData = result;
			}
			else if (std::endian::native == std::endian::big)
			{
				Read(&outData, sizeof(outData));
			}
		}
		//32
		void Read(int32_t& outData) {
			if (std::endian::native == std::endian::little)
			{
				Read(&outData, sizeof(outData));
				int32_t result = ByteSwap(outData);
				outData = result;
			}
			else if (std::endian::native == std::endian::big)
			{
				Read(&outData, sizeof(outData));
			}
		}
		//64
		void Read(int64_t& outData) {
			if (std::endian::native == std::endian::little)
			{
				Read(&outData, sizeof(outData));
				int64_t result = ByteSwap(outData);
				outData = result;
			}
			else if (std::endian::native == std::endian::big)
			{
				Read(&outData, sizeof(outData));
			}
		}


		//32
		void Read(float& outData) {
			if (std::endian::native == std::endian::little)
			{
				Read(&outData, sizeof(outData));
				float result = ByteSwap(outData);
				outData = result;
			}
			else if (std::endian::native == std::endian::big)
			{
				Read(&outData, sizeof(outData));
			}
		}

		//64
		void Read(double& outData) {
			if (std::endian::native == std::endian::little)
			{
				Read(&outData, sizeof(outData));
				double result = ByteSwap(outData);
				outData = result;
			}
			else if (std::endian::native == std::endian::big)
			{
				Read(&outData, sizeof(outData));
			}
		}

		void Read(std::wstring& outData)
		{
			//string ±Ê¿Ã πﬁæ∆ø»
			int16_t length = 0;
			Read(length);
			//string ±Ê¿Ã ¥√∑¡µ“
			outData.resize(length);

			if (std::endian::native == std::endian::little)
			{
				Read(&outData[0], length*sizeof(wchar_t));
			}
			else if (std::endian::native == std::endian::big)
			{
				//std::reverse(outData.begin(), outData.end());
				Read(&outData[0], length);
			}
		}

		template<class T>
		void Read(std::vector<T>& vector)
		{
			int32_t size = 0;
			Read(size);

			vector.resize(size);
			if (std::endian::native == std::endian::little)
			{
				Read(&vector[0], size * sizeof(T));
			}
			else if (std::endian::native == std::endian::big)
			{
				Read(&vector[0], size * sizeof(T));
			}
		}

		void Read(nlohmann::json& json)
		{
			int32_t size = 0;
			Read(size);

			std::string str;
			str.resize(size);
			if (std::endian::native == std::endian::little)
			{
				Read((void*)str.c_str(), size);
				std::reverse(str.begin(), str.end());
			}
			else if (std::endian::native == std::endian::big)
			{
				Read((void*)str.c_str(), size);
			}
			json = nlohmann::json::parse(str);
				
		}
		
	};
}