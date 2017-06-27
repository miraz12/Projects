#include "stdafx.h"
#include "networkhandler.h"
#include <stdio.h>
#include <iostream>
#include <thread>
#include "messagelistener.h"



#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define DEFAULT_PORT "49152"
#define DEFAULT_BUFLEN 512

NetworkHandler::NetworkHandler(Client* inClient) : client1(inClient)
{
	client2 = new Client();

	//structure contains information about the Windows Sockets implementation.
	WSADATA wsaData;
	iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return;
	}
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	std::string sargv = "130.240.40.26";
	const char* argv = sargv.c_str();

	iResult = getaddrinfo(argv, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return;
	}

	ConnectSocket = INVALID_SOCKET;

	ptr = result;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	// Connect to server.
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		return;
	}
}

void NetworkHandler::listener()
{
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN];

	// Receive data until the server closes the connection
	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			printf("Bytes received: %d\n", iResult);
			MsgHead* msghead;
			msghead = (MsgHead*)recvbuf;

			std::cout << "id " << msghead->id << " length " << msghead->length << " sekvensnummer " << msghead->seq_no << std::endl;

			this->client1->seq_nr = msghead->seq_no;			

			if (msghead->type == Join)
			{
				std::cout << "Join message\n";

				//Cast to change msg
				JoinMsg* msgType;
				msgType = (JoinMsg*)recvbuf;

				if (this->client1->client_id == -1)
				{
					std::cout << "Joined server. \n";
					this->client1->client_id = msghead->id;
				}
				else if (this->client2->client_id == -1)
				{
					std::cout << "Opponent joined. \n";
					this->client1->client_id = msghead->id;
				}
				else
				{
					std::cout << "Max players already in game. \n";
				}

			}
			if (msghead->type == TextMessage)
			{
				std::cout << "text message\n";
			}
			if (msghead->type == Leave)
			{
				std::cout << "player leave message\n";
				// shutdown the connection for sending since no more data will be sent
				// the client can still use the ConnectSocket for receiving data
				iResult = shutdown(ConnectSocket, SD_SEND);
				if (iResult == SOCKET_ERROR) 
				{
					printf("shutdown failed: %d\n", WSAGetLastError());
					closesocket(ConnectSocket);
					WSACleanup();
					return;
				}
				break;
			}
			if (msghead->type == Change)
			{
				//Cast to change msg
				ChangeMsg* msgType;
				msgType = (ChangeMsg*)recvbuf;
				std::cout << "Change message\n";

				if (msgType->type == NewPlayer)
				{
					NewPlayerMsg* type = (NewPlayerMsg*)msgType;
					std::cout << "New player joined\n";
				}
				if (msgType->type == PlayerLeave)
				{
					PlayerLeaveMsg* type = (PlayerLeaveMsg*)msgType;
					std::cout << "Player left\n";
				}
				if (msgType->type == NewPlayerPosition)
				{
					NewPlayerPositionMsg* type = (NewPlayerPositionMsg*)msgType;
					if (msgType->head.id == client1->client_id)
					{
						client1->pos = type->pos;
						std::cout << "New this player position: x:" << client1->pos.x << " y: " << client1->pos.y << "\n";
					}
					else if (msgType->head.id == client2->client_id)
					{
						client2->pos = type->pos;
						std::cout << "New enemy player position: x:" << client1->pos.x << " y: " << client1->pos.y << "\n";
					}
				}
			}
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (iResult > 0);
}

NetworkHandler::~NetworkHandler()
{

}
