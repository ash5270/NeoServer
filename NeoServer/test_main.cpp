﻿#pragma once
#include "network/HttpClient.h"
#include "network/IOCPServer.h"

#include "system/InputMemoryStream.h"
#include "system/OutputMemoryStream.h"
#include "Common.h"

#include <locale>
#include <json.h>

using namespace std;


int main()
{
	/*neo::system::Buffer buffer(1024);

	neo::system::OutputMemoryStream out(buffer);

	int32_t num = 0x12345678;
	out.Write(num);
	wstring str = L"str";
	out.Write(str);

	neo::system::InputMemoryStream in(buffer);

	int32_t n;
	wstring s;

	in.Read(n);
	in.Read(s);*/


	//uint32_t  x = 0x123456789;



	//int sum = 0;
	//func(10, sum);

	//SetConsoleOutputCP(CP_UTF8);
	//setvbuf(stdout, nullptr, _IOFBF, 1000);
	//_wsetlocale(LC_ALL, L"korean");
	//
	//neo::network::HttpClient client("http://127.0.0.1", 3000);
	//client.InitHttp();

	//Json::StreamWriterBuilder builder;

	//Json::Value pizzaData;
	//pizzaData["name"] = u8"아니 왜 안와 이번에는 이게 머야";
	//pizzaData["isGlutenFree"] = false;

	//Json::Value updatePizza;
	//updatePizza["id"] = u8"1";
	//updatePizza["name"] = u8"아니 왜 안와 이번에는 이게 머야";
	//updatePizza["isGlutenFree"] = false;
	//	
	//neo::network::HttpData data;
	//client.Put("/Pizza/1", Json::writeString(builder, updatePizza));

	//std::string out;
	////auto response= client.Post("/Pizza", Json::writeString(builder, pizzaData));
	//std::string out2;

	//auto response= client.Get("/Login");
	//if(response.HttpState==200)
	//{
	//	out2 = response.Body;
	//}

	////client.Delete("/Pizza/3");
	//Json::Reader reader;
	//Json::Value body;
	//reader.parse(out2, body, false);

	//for (int i = 0; i < body.size(); i++)
	//{
	//	wstring te = Utf8ToUnicode(body[i]["id"]);
	//	wstring hash = Utf8ToUnicode(body[i]["hashID"]);
	//	wcout << " A:" << te << ":" <<hash << "\n";
	//}


	using namespace  neo::network;
	IOCPServer server;
	server.InitializeServer(4560);
	server.StartServer();

	while (true)
	{
		
	}

	return 0;
}