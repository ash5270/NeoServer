#pragma once

//winsock2를 사용하기 위해 아래 코멘트 추가 
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>

#include"../system/NeoLog.h"

const size_t Memory_Pool_Block_Size = 1024;
const size_t Memory_Pool_Block_Count = 300;
