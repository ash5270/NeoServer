#pragma once

#include <string>
#include <unordered_map>

namespace network
{
	struct HttpData
	{
		std::string HttpVersion;
		int HttpState;
		std::string HttpStateMsg;
		std::unordered_map<std::string, std::string> Headers;
	};
}