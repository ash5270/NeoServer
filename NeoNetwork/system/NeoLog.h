#pragma once

#include <string>
#include <stdexcept>


#include "LogSystem.h"
#include "LogData.h"

template<typename... Args>
std::wstring wstring_format(const wchar_t* format, Args... args)
{
	//len은 가변배열을 다 받아온 길이를 가져오게 된다.
	const int len = swprintf(nullptr, 0, format, args...) + 1;
	if (len <= 0)
	{
		throw std::runtime_error("Error during string format...");
	}

	auto size = static_cast<size_t>(len);
	auto buffer = std::make_unique<wchar_t[]>(size);
	_snprintf_s(buffer.get(), size, size, format, args...);


	
	return std::wstring(buffer.get(), buffer.get() + size - 1);
}

#define LOG_PRINT(LOG_TYPE,format,...)\
	{\
		auto str= wstring_format(format,##__VA_ARGS__); \
		neo::system::LogData logData(LOG_TYPE,str)\
		neo::system::LogSystem::GetInstance().AddLogData(std::move(logData));\
	}\
	