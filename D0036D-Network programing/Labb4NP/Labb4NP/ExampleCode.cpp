//============================================================================
// Name        : W_C_Soc_sending_joinmsg.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Example sending join message to server using windows socket api (WSA)
//============================================================================

#include "stdafx.h" 
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include "structs.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define DEFAULT_PORT "5555"
#define DEFAULT_BUFLEN 512
using namespace std;
int stuff()
{
	//structure contains information about the Windows Sockets implementation.
	WSADATA wsaData;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	string sargv = "cloud-44.skelabb.ltu.se";
	const char* argv = sargv.c_str();

	iResult = getaddrinfo(argv, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;

	ptr = result;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Connect to server.
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}
	JoinMsg join;
	int recvbuflen = DEFAULT_BUFLEN;

	char sendbuf[sizeof(join)];
	char recvbuf[DEFAULT_BUFLEN];

	//filling join message
	join.head.id = 0;
	join.head.length = sizeof(join);
	join.head.type = Join;

	//copying join message to sendbuf
	memcpy((void*)sendbuf, (void*)&join, sizeof(join));

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, sizeof(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection for sending since no more data will be sent
	// the client can still use the ConnectSocket for receiving data
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Receive data until the server closes the connection
	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			printf("Bytes received: %d\n", iResult);
			MsgHead* msghead;
			msghead = (MsgHead*)recvbuf;
			cout << "id " << msghead->id << " length " << msghead->length << " sekvensnummer " << msghead->seq_no << endl;
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (iResult > 0);

	cout << "end" << endl;
	return 0;
}