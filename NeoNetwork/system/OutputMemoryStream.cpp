#include "OutputMemoryStream.h"

neo::system::OutputMemoryStream::OutputMemoryStream(const Buffer& buffer) : MemoryStream(buffer)
{
}

neo::system::OutputMemoryStream::OutputMemoryStream(char* buffer, const size_t& capacity):MemoryStream(buffer,capacity)
{
	
}

neo::system::OutputMemoryStream::~OutputMemoryStream()
{

}

void neo::system::OutputMemoryStream::Write(const void* inData, size_t inByteSize)
{
	//공간 확보한다.
	int32_t resultHead = mHead + static_cast<uint32_t>(inByteSize);
	if (resultHead > mCapacity)
		wprintf(L"ERROR : Buffer overflow\n");
	//버퍼의 제일 앞에 복사
	std::memcpy(mBuffer + mHead, inData, inByteSize);
	//mHead를 전진시켜 다음 기록에 대비
	mHead = resultHead;
}
