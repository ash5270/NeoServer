//바이트 스왑
//네트워크 바이트 오더는 빅엔디안이기 때문에 
//빅엔디안으로 고정시켜서 보내기 위한 변환 헤더
//big_endian to littie_endian convert
//littie_endian to big_endian convert

#pragma once
#include <stdint.h>

namespace neo::system
{
	//16bit convert
	static uint16_t SwapByte16(uint16_t data)
	{
		return (data >> 8) | (data << 8);
	}

	//32bit convert
	static uint32_t SwapByte32(uint32_t data)
	{
		return	((data >> 24) & 0x000000FF) |
			((data >> 8) & 0x0000FF00) |
			((data << 8) & 0x00FF0000) |
			((data << 24) & 0xFF000000);
	}

	//64bit convert
	static uint64_t SwapByte64(uint64_t data)
	{
		return	((data >> 56) & 0x00000000000000FF) |
			((data >> 40) & 0x000000000000FF00) |
			((data >> 24) & 0x0000000000FF0000) |
			((data >> 8) & 0x00000000FF000000) |
			((data << 8) & 0x000000FF00000000) |
			((data << 24) & 0x0000FF0000000000) |
			((data << 40) & 0x00FF000000000000) |
			((data << 56) & 0xFF00000000000000);
	}

	//tpye에 대한 별칭 클래스를 만들어둠
	template<class FromType, class ToType>
	class TypeAliaser {
	public:
		TypeAliaser(FromType data) : mFromTypeData(data) {}
		ToType& Get() { return mToTypeData; }

		//union을 통해서 convert
		union
		{
			FromType mFromTypeData;
			ToType mToTypeData;
		};
	};

	//사이즈를 자동으로 해주기 판단해주기 위한 템플릿
	template<typename T, size_t size>
	class ByteSwapper;

	template<typename T>
	class ByteSwapper<T, 2>
	{
	public:
		T Swap(T data) const {
			uint16_t result = SwapByte16(TypeAliaser<T, uint16_t>(data).Get());
			return TypeAliaser<uint16_t, T>(result).Get();
		}
	};

	template<typename T>
	class ByteSwapper<T, 4>
	{
	public:
		T Swap(T data) const {
			uint32_t result = SwapByte32(TypeAliaser<T, uint32_t>(data).Get());
			return TypeAliaser<uint32_t, T>(result).Get();
		}
	};

	template<typename T>
	class ByteSwapper<T, 8>
	{
	public:
		T Swap(T data) const {
			uint64_t result = SwapByte64(TypeAliaser<T, uint64_t>(data).Get());
			return TypeAliaser<uint64_t, T>(result).Get();
		}
	};

	template<typename T>
	T ByteSwap(T data)
	{
		return ByteSwapper<T, sizeof(T)>().Swap(data);
	}

}