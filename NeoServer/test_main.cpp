#pragma once
#include "network/HttpClient.h"
#include "network/IOCPServer.h"
#include "Common.h"

#include <locale>
#include <json.h>
#include <Windows.h>
#include <iostream>
using namespace std;



int func(int num,int& sum)
{
	sum = num + num;
	return  num + 1; 
}

int main()
{
	//int sum = 0;
	//func(10, sum);

	////SetConsoleOutputCP(CP_UTF8);
	////setvbuf(stdout, nullptr, _IOFBF, 1000);
	//_wsetlocale(LC_ALL, L"korean");
	//
	//network::HttpClient client("http://127.0.0.1", 3000);
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
	//network::HttpData data;
	//client.Put("/Pizza/1", Json::writeString(builder, updatePizza));

	//std::string out;
	////auto response= client.Post("/Pizza", Json::writeString(builder, pizzaData));
	//std::string out2;

	//auto response= client.Get("/Pizza");
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
	//	wstring te = Utf8ToUnicode(body[i]["name"]);
	//	wcout << " A:" << te << "  " << body[i]["isGlutenFree"].asBool() << "\n";
	//}


	using namespace  neo::network;
	IOCPServer server;
	server.InitializeServer(3000);
	server.StartServer();

	while (true)
	{
		
	}

	return 0;
}