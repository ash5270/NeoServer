#include "OutputMemoryStream.h"
neo::system::OutputMemoryStream::OutputMemoryStream() : MemoryStream()
{

}

neo::system::OutputMemoryStream::~OutputMemoryStream()
{

}

void neo::system::OutputMemoryStream::Write(const void* inData, size_t inByteSize)
{

	//���� Ȯ���Ѵ�.
	uint32_t resultHead = mHead + static_cast<uint32_t>(inByteSize);
	if (resultHead > mCapacity)
		ReallocMemory(std::max(mCapacity * 2, resultHead));

	//������ ���� �տ� ����
	std::memcpy(mBuffer + mHead, inData, inByteSize);
	//mHead�� �������� ���� ��Ͽ� ���
	mHead = resultHead;

}
