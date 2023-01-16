#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

namespace network
{
	struct HttpData
	{
		HttpData()
		{
			std::cout << "생성자\n";
		}

		std::string HttpVersion;
		int HttpState;
		std::string HttpStateMsg;
		std::unordered_map<std::string, std::string> Headers;
		std::string Body;
	};
}