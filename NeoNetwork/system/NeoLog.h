﻿#pragma once

#include <string>
#include <stdexcept>


#include "LogSystem.h"
#include "LogData.h"

typedef neo::system::LogType LOG_LEVEL;

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
	_snwprintf_s(buffer.get(), size, size, format, args...);

	return std::wstring(buffer.get(), buffer.get() + size - 1);
}
//여기를 클래스로 만들어야할듯 
#define LOG_PRINT(LOG_TYPE,format,...)\
neo::system::LogSystem::GetInstance().OutPutLog(LOG_TYPE,\
	wstring_format(format, ##__VA_ARGS__))\

