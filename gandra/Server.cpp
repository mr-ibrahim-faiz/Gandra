#include "Server.h"
#include<iostream>

SOCKET server() {
	WSADATA wsadata;
	
	int iResult;

	struct addrinfo *result = NULL,
		hints;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	//Initialize WinSock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return INVALID_SOCKET;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	//Resolve local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error; %d\n", iResult);
		WSACleanup();
		return INVALID_SOCKET;
	}

	//Create a socket to listen on the client connection
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return INVALID_SOCKET;
	}

	//Bind the local address with the socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}

	freeaddrinfo(result);

	//Listen on the socket
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}

	//Accept Client connection
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}

	closesocket(ListenSocket);

	return ClientSocket;
}

SOCKET client(string string_ip_adress) {
	WSADATA wsadata;

	int iResult;

	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	SOCKET ConnectSocket = INVALID_SOCKET;

	//Initialize WinSock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return INVALID_SOCKET;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	
	PCSTR ip_address = string_ip_adress.c_str();

	iResult = getaddrinfo(ip_address, DEFAULT_PORT, &hints, &result); //Test
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return INVALID_SOCKET;
	}

	//// Attempt to connect to an address until one succeeds
	//for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

	//	// Create a SOCKET for connecting to server
	//	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
	//		ptr->ai_protocol);
	//	if (ConnectSocket == INVALID_SOCKET) {
	//		printf("socket failed with error: %ld\n", WSAGetLastError());
	//		WSACleanup();
	//		return 1;
	//	}

	//	// Connect to server.
	//	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	//	if (iResult == SOCKET_ERROR) {
	//		closesocket(ConnectSocket);
	//		ConnectSocket = INVALID_SOCKET;
	//		continue;
	//	}
	//	break;
	//}

	ptr = result;

	//Create socket to connect to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return INVALID_SOCKET;
	}

	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult != 0) {
		printf("connect failed with error: %d\n", iResult);
		freeaddrinfo(result);
		WSACleanup();
		return INVALID_SOCKET;
	}

	return ConnectSocket;
}