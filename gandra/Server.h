#pragma once

#include<WinSock2.h>
#include<WS2tcpip.h>

#include<string>
using std::string;

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

#pragma comment(lib,"Ws2_32.lib")

SOCKET server();
SOCKET client(string);