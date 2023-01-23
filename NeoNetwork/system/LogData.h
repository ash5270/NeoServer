﻿//로그 출력을 위한 로그 데이터  
//
#pragma once
#include <string>
namespace neo::system
{
	//LOG Type
	enum class LogType 
	{
		//확인을 위한 용도
		INFO=0,
		//경고
		WARNING,
		//에러
		ERROR,
		//디버그 용도
		DEBUG,
	};

	//Log Data 
	struct LogData
	{
		LogType Type;
		std::wstring LogMessage;
	};
}