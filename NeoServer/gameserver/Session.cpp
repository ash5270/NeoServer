#include "Session.h"
#include <system/OutputMemoryStream.h>
#include <system/InputMemoryStream.h>
#include "../packet/PacketObject.h"
#include "../packet/PacketAnalyzeAndCreate.h"
#include"../manager/ChannelManager.h"
#include "GameServer.h"


neo::server::Session::Session() : IOCPSession(), mChannelId(0),mGameServer(nullptr), mRecvOffset(0)
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

void neo::server::Session::SetGameServer( GameServer* gameServer)
{
	mGameServer = gameServer;
}

void neo::server::Session::OnSend(size_t transferSize)
{
	//LOG_PRINT(LOG_LEVEL::LOG_INFO, L"--OnSend %d \n", transferSize);
	//사용한 버퍼 다시 해제
	auto buf = mSendData->GetBuffer();
	mMemoryPool->Free(buf);
	
	if (mSendPacketQueue.IsEmpty())
	{
		bool check = true;
		mIsSending.compare_exchange_strong(check,false);
	}
	else
	{
		SendIO();
	}

	//LOG_PRINT(LOG_LEVEL::LOG_ERROR, L"missending %d\n", mIsSending.load());
}

void neo::server::Session::OnRecv(size_t transferSize)
{
	//LOG_PRINT(LOG_LEVEL::LOG_INFO, L"--OnRecv %d \n", transferSize);
	InputMemoryStream inputStream(mRecvData->GetBuffer(), Memory_Pool_Block_Size);
	while (inputStream.GetLength() < transferSize)
	{
		//packet create
		auto newPacket = packet::PacketAnalyzeAndCreate::GetInstance().Analyzer(inputStream,transferSize);
		//패킷을 해석할 수 없거나, 받은 데이터 사이즈가 패킷 크기보다 작을 경우 
		if (newPacket == nullptr)
		{
			//현재 패킷을 해독 할 수 없음
			//근데 지금 이미 패킷을 new 했기때문에 손해일듯 
			//남는 패킷 사이즈 만큼 저장해두고 앞쪽으로 데이터 옮겨 두기
			auto leftSize = transferSize - inputStream.GetLength();
			LOG_PRINT(LOG_LEVEL::LOG_DEBUG, L"left size ==  %d\n", leftSize);
			mRecvOffset = leftSize;
			break;

		}	
		else {
			packet::PacketObject* packetObj = new packet::PacketObject();
			packetObj->packet = newPacket;
			packetObj->session = this;

			//Packet을 queue에 넣음
			auto channel = static_cast<int>(newPacket->Channel);
			mGameServer->GetChannelThread()[channel]->GetPacketQueue()->Enqueue(packetObj);
			//LOG_PRINT(LOG_LEVEL::LOG_INFO, L"Recv Packet id : %d,  size : %d, channel : %d\n", (int)newPacket->GetID(), newPacket->GetSize(), channel);
			mRecvOffset = 0;
		}
	}
	
	if (mRecvOffset!=0)
		this->RecvReady(inputStream.GetLength(), mRecvOffset);
	else
		this->RecvReady();  
}

void neo::server::Session::OnClose ()
{
	LOG_PRINT(LOG_LEVEL::LOG_INFO, L"OnClosed\n");
	neo::object::ChannelManager::GetInstance().RemoveUser(mChannelId, this);
	mIsConneting.exchange(false);
}



void neo::server::Session::SendPacket(Packet& packet)
{
	//메모리풀에서 하나 꺼내옴
	if (!mIsConneting.load())
		return;

	auto buffer = mMemoryPool->Aollocate();
	system::OutputMemoryStream output(buffer, mMemoryPool->GetBlockSize());
	packet.Serialize(output);
	WSABUF buf;
	buf.buf = buffer;
	buf.len = output.GetLength();
	mSendPacketQueue.enqueue(buf);
	bool check = false;
	if (mIsSending.compare_exchange_strong(check, true))
	{
		SendIO();
	}
}
