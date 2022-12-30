//http 통신을 위한 client 
#pragma once
#include <string>
#include <locale>
#include <sstream>

#include "SocketCommon.h"
#include "HttpData.h"

namespace network
{
	class HttpClient
	{
	public:
		HttpClient();
		HttpClient(const std::string& serverUrl,const int& port);
		~HttpClient();

		bool InitHttp();
		bool CloseHttp();

		void Get(const std::string& url, std::string& outputData);
		void Post(const std::string& url, const std::string& data, std::string& outputData);
		void Put(const std::string& url, const std::string& data);
		void Delete(const std::string& url, const std::string& data);

	private:
		bool WSAInit();
		void parseUrl(std::string& url, std::string& serverName, std::string& fileName, std::string& filePath, int& port);
		HttpData parseResponseData(const std::string& response, std::string& outBodyData);
		std::ostringstream setConnection(const std::string& url, const std::string& httpMethod);

	private:
		SOCKADDR_IN mSockAddr;
		SOCKET mSocket;
		bool mIsConnect;
		std::string mServerUrl;
		int mPort;
	};
}
