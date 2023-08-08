#pragma once
#include <string>
#include <Windows.h>
#include <optional>

//utf8으로 인코딩 된것을 unicode를 바꿔줌
std::optional<std::wstring> Utf8ToUnicode(const std::string& utf8)
{
	std::wstring result;
	if (utf8.size() < 0)
		return result;
	int nLen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), strlen(utf8.c_str()), NULL, NULL);
	if (nLen < 0)
		return  std::nullopt;
	//reserve사용시 wchar_t * nLen 길이만큼 메모리를 할당해버림
	result.resize(nLen);
	MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(),
		strlen(utf8.c_str()), &result[0], nLen);
	return result;
}

std::optional<std::string> UnicodeToUtf8(const std::wstring& unicode)
{
	std::string result;
	if (unicode.size() < 0)
		return std::nullopt;
	int nLen = WideCharToMultiByte(CP_UTF8, 0, unicode.c_str(), lstrlenW(unicode.c_str()), NULL, 0, NULL, NULL);
	result.resize(nLen);
	WideCharToMultiByte(CP_UTF8, 0, unicode.c_str(), lstrlenW(unicode.c_str()), &result[0], nLen,NULL,NULL);
	return result;
}