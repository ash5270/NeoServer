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
	//���� Ȯ���Ѵ�.
	int32_t resultHead = mHead + static_cast<uint32_t>(inByteSize);
	if (resultHead > mCapacity)
		wprintf(L"ERROR : Buffer overflow\n");
	//������ ���� �տ� ����
	std::memcpy(mBuffer + mHead, inData, inByteSize);
	//mHead�� �������� ���� ��Ͽ� ���
	mHead = resultHead;
}
