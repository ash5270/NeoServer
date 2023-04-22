//http 통신을 위한 client 
#pragma once
#include <string>
#include <locale>
#include <sstream>
#include <functional>

#include "SocketCommon.h"
#include "HttpData.h"

namespace neo::network
{
	class HttpClient
	{
	public:
		HttpClient();
		HttpClient(const std::string& serverUrl,const int& port);
		~HttpClient();

		bool InitHttp();
		bool CloseHttp();

		HttpData Get(const std::string& url);
		HttpData Post(const std::string& url, const std::string& data);
		HttpData Put(const std::string& url, const std::string& data);
		HttpData Delete(const std::string& url);

	private:
		bool WSAInit();
		void parseUrl(std::string& url, std::string& serverName, std::string& fileName, std::string& filePath, int& port);
		void closeSocket();
		
		HttpData parseResponseData(const std::string& response);
		std::ostringstream setConnection(const std::string& url, const std::string& httpMethod);

	private:
		SOCKADDR_IN mSockAddr;
		SOCKET mSocket;
		bool mIsConnect;
		std::string mServerUrl;
		int mPort;
	};
}
