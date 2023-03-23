#include "InputMemoryStream.h"


neo::system::InputMemoryStream::InputMemoryStream( char* buffer,const uint32_t& capacity):
	MemoryStream(buffer,capacity)
{

}

neo::system::InputMemoryStream::~InputMemoryStream()
{

}

void neo::system::InputMemoryStream::Read(void* outData, uint32_t inByteSize)
{
	uint32_t resultHead = mHead + static_cast<uint32_t>(inByteSize);
	memcpy(outData,mBuffer + mHead, inByteSize);

	//mHead를 읽으면서 앞으로 옮김
	mHead = resultHead;
}
