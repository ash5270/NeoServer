#pragma once
#include "MemoryStream.h"
#include <string>

namespace neo::system
{
	class OutputMemoryStream : public MemoryStream
	{
	public:
		OutputMemoryStream(const Buffer& buffer);
		OutputMemoryStream(char* buffer, const size_t& capacity);
		~OutputMemoryStream();

		//�⺻���� �����ͷ� ������ ����
		void Write(const void* inData, size_t inByteSize);
		//���ø����� ������ ����
		template<typename T>
		void Write(const T& inData)
		{
			//�����ڷ����� ������
			static_assert(std::is_arithmetic<T>::value 
				|| std::is_enum<T>::value,
				"Generic Write only supports primitives data types");
			Write(&inData, sizeof(inData));
		}
	public:
		//8
		void Write(const char& inData)
		{
			Write(&inData, sizeof(inData));
		}
		//16
		void Write(const int16_t& inData)
		{
			if (std::endian::native == std::endian::little)
			{
				int16_t result = ByteSwap(inData);
				Write(&result, sizeof(inData));
			}
			else if (std::endian::native == std::endian::big)
			{
				Write(&inData, sizeof(inData));
			}
		}
		//32
		void Write(const int32_t& inData)
		{
			if (std::endian::native == std::endian::little)
			{
				int32_t result = ByteSwap(inData);
				Write(&result, sizeof(inData));
			}
			else if (std::endian::native == std::endian::big)
			{
				Write(&inData, sizeof(inData));
			}
		}
		//64
		void Write(const int64_t& inData)
		{
			if (std::endian::native == std::endian::little)
			{
				int64_t result = ByteSwap(inData);
				Write(&result, sizeof(inData));
			}
			else if (std::endian::native == std::endian::big)
			{
				Write(&inData, sizeof(inData));
			}
		}
		//32
		void Write(const float& inData)
		{
			if (std::endian::native == std::endian::little)
			{
				float result = ByteSwap(inData);
				Write(&result, sizeof(inData));
			}
			else if (std::endian::native == std::endian::big)
			{
				Write(&inData, sizeof(inData));
			}
		}
		//64
		void Write(const double& inData)
		{
			if (std::endian::native == std::endian::little)
			{
				double result = ByteSwap(inData);
				Write(&result, sizeof(inData));
			}
			else if (std::endian::native == std::endian::big)
			{
				Write(&inData, sizeof(inData));
			}
		}

		//string 
		void Write(const std::wstring& inData) {
			//string ���̺��� 
			//wchart�� ���� ��ŭ ���̷� ����
			int16_t length = inData.size() * sizeof(wchar_t);
			Write(&length, sizeof(length));
			//string write
			Write(inData.c_str(), length);
		}
	};
}



