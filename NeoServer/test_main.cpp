#include"./packet/NeoPacketFactory.h"
#include<iostream>
#include <Windows.h>
#include<UtfToUnicode.h>

using namespace google::protobuf::io; 
 
int main()
{
	std::locale::global(std::locale("kor")); //"UTF-8") );
	char* buffer = new char[1024];
	char test[] = "ash5270";


	neo::P_C_REQ_LOGIN loginPakcet;
	std::wstring name = L"æ»≥Á«œººø‰";
	std::string utfName = UnicodeToUtf8(name).value();
	//std::string convertName = "æ»≥Á«œººø‰";
	loginPakcet.set_id(utfName);
	loginPakcet.set_uuid("111111111111");

	neo::NeoPacket packet;
	packet.header.packetID = neo::PacketID::PI_C_REQ_LOGIN;
	packet.header.packetSize = loginPakcet.ByteSizeLong();
	
	::ArrayOutputStream outArray(buffer, 1024);
	::CodedOutputStream outCoded(&outArray);
	outCoded.WriteRaw(&packet.header, sizeof(neo::NeoPacketHeader));
	loginPakcet.SerializeToCodedStream(&outCoded);
	auto count = outCoded.ByteCount();
	::ArrayInputStream arrayStream(buffer, loginPakcet.ByteSizeLong()+sizeof(neo::NeoPacketHeader));
	::CodedInputStream codedInputStream(&arrayStream);
	std::wstring copy;
	auto resPacket= neo::packet::PacketFactory::GetInstance().CreateMessage(codedInputStream);
	if (resPacket.has_value())
	{
		auto resHeader = resPacket.value().header;
		auto ptr = resPacket.value().payload.get();
		auto loginRes = dynamic_cast<neo::P_C_REQ_LOGIN*>(ptr);
		auto wname = Utf8ToUnicode(loginRes->id());
		copy = wname.value();
		//std::cout << resHeader.packetID << ", " << resHeader.packetSize <<"\n";
	}

	std::wcout << copy;
	return 0;
}