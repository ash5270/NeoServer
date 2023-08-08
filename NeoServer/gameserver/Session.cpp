#include "Session.h"
#include <system/OutputMemoryStream.h>
#include <system/InputMemoryStream.h>
#include "../packet/PacketObject.h"
#include "../packet/PacketAnalyzeAndCreate.h"
#include "../packet/NeoPacket.h"
#include"../packet/NeoPacketFactory.h"
#include"../manager/ChannelManager.h"
#include "GameServer.h"


neo::server::Session::Session() : IOCPSession(), mChannelId(0), mGameServer(nullptr), mRecvOffset(0)
{

}

neo::server::Session::~Session()
{

}

bool neo::server::Session::OnAccept(TCPSocket* socket, SocketAddress* addrInfo)
{
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"OnAccept\n");

	mTCPSocket = socket;
	mSocketAddress = addrInfo;

	mIsConneting.store(true);
	mIsSending.store(false);



	return true;
}

void neo::server::Session::SetGameServer(GameServer* gameServer)
{
	mGameServer = gameServer;
}

void neo::server::Session::OnSend(size_t transferSize)
{
	//LOG_PRINT(LOG_LEVEL::LOG_INFO, L"--OnSend %d \n", transferSize);
	//����� ���� �ٽ� ����
	auto buf = mSendData->GetBuffer();
	mMemoryPool->Free(buf);

	if (mSendPacketQueue.IsEmpty())
	{
		bool check = true;
		mIsSending.compare_exchange_strong(check, false);
	}
	else
	{
		SendIO();
	}

	//LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"missending %d\n", mIsSending.load());
}

//���� ������ ����
//��Ŷ ����� �ƴ� ���� ����Ʈ �������ϻ�
void neo::server::Session::OnRecv(size_t transferSize)
{
	//LOG_PRINT(LOG_LEVEL::LOG_INFO, L"--OnRecv %d \n", transferSize);
	InputMemoryStream inputStream(mRecvData->GetBuffer(), Memory_Pool_Block_Size);
	while (inputStream.GetLength() < transferSize)
	{
		//packet create
		auto newPacket = packet::PacketFactory::GetInstance().AnalyzerStream(inputStream, transferSize);
		//��Ŷ�� �ؼ��� �� ���ų�, ���� ������ ����� ��Ŷ ũ�⺸�� ���� ��� 
		if (!newPacket.has_value())
		{
			//���� ��Ŷ�� �ص� �� �� ����s
			//�ٵ� ���� �̹� ��Ŷ�� new �߱⶧���� �����ϵ� 
			//���� ��Ŷ ������ ��ŭ �����صΰ� �������� ������ �Ű� �α�
			auto leftSize = transferSize - inputStream.GetLength();
			LOG_PRINT(LOG_LEVEL::LOG_DEBUG, L"left size ==  %d\n", leftSize);
			mRecvOffset = leftSize;
			break;

		}
		else {
			int32_t channel = newPacket.value().header.channel;

			neo::PacketObjPtr packetObjPtr;
			packetObjPtr->packet = std::move(newPacket.value());
			packetObjPtr->session = std::unique_ptr<Session>(this);

			//Packet�� queue�� ����
			mGameServer->GetChannelThread()[channel]->GetInsertQueue().push(std::move(packetObjPtr));
			//LOG_PRINT(LOG_LEVEL::LOG_INFO, L"Recv Packet id : %d,  size : %d, channel : %d\n", (int)newPacket->GetID(), newPacket->GetSize(), channel);
			mRecvOffset = 0;
		}
	}

	if (mRecvOffset != 0)
		this->RecvReady(inputStream.GetLength(), mRecvOffset);
	else
		this->RecvReady();
}

void neo::server::Session::OnClose()
{
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"OnClosed\n");
	neo::object::ChannelManager::GetInstance().RemoveUser(mChannelId, this);
	mIsConneting.exchange(false);
}



void neo::server::Session::SendPacket(neo::NeoPacket& packet)
{
	//�޸�Ǯ���� �ϳ� ������
	if (!mIsConneting.load())
		return;
	//�̷��� ������ �����ٿ�
	//queue �ΰ��� �Ἥ ����ص� �ɵ�
	const auto buffer = mMemoryPool->Aollocate();
	//system::OutputMemoryStream output(buffer, mMemoryPool->GetBlockSize());
	io::ArrayOutputStream arrayStream(buffer, mMemoryPool->GetBlockSize());
	io::CodedOutputStream codedStream(&arrayStream);

	codedStream.WriteRaw(&packet.header, sizeof(packet.header));
	packet.payload->SerializeToCodedStream(&codedStream);

	WSABUF buf;
	buf.buf = buffer;
	buf.len = codedStream.ByteCount();
	mSendPacketQueue.enqueue(buf);
	bool check = false;
	if (mIsSending.compare_exchange_strong(check, true))
	{
		SendIO();
	}
}


