#include "networklistener.h"
#include <iostream>
#include <stdio.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library
#define DEFAULT_BUFLEN 1024

NetworkListener::NetworkListener()
{
}

NetworkListener::NetworkListener(Client* inClient, Client* inEnemy, SOCKET sock, RC4* rc4in, RC4* rc4out) : client(inClient), enemy(inEnemy), ConnectSocket(sock)
{
	rc4In = rc4in;
	rc4Out = rc4out;

	join();
 }


NetworkListener::~NetworkListener()
{
}

void NetworkListener::listener()
{
	char* recvbuf = new char[DEFAULT_BUFLEN];

	// Receive data until the server closes the connection
	do {
		iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);

		if (iResult > 0)
		{
			this->rc4In->Decrypt(recvbuf, iResult);

			printf("Bytes received: %d\n", iResult);
			MsgHead msghead;
			memcpy(&msghead, recvbuf, sizeof(MsgHead));

			std::cout << "id " << msghead.id << " length " << msghead.length << " sekvensnummer " << msghead.seq_no << std::endl;

			this->client->seq_nr = msghead.seq_no;

			if (msghead.type == Join)
			{
				std::cout << "Join message\n";

				//Cast to change msg
				JoinMsg msgType;
				memcpy(&msgType, recvbuf, sizeof(JoinMsg));

				if (this->client->client_id == -1)
				{
					std::cout << "Joined server. \n";
					this->client->client_id = msghead.id;
				}
				else if (this->enemy->client_id == -1)
				{
					std::cout << "Opponent joined. \n";
					this->enemy->client_id = msghead.id;
				}
				else
				{
					std::cout << "Max players already in game. \n";
				}

			}
			if (msghead.type == TextMessage)
			{
				std::cout << "text message\n";
			}
			if (msghead.type == Leave)
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
			if (msghead.type == Change)
			{
				//Cast to change msg
				ChangeMsg msgType;
				memcpy(&msgType, recvbuf, sizeof(ChangeMsg));

				std::cout << "Change message\n";

				if (msgType.type == NewPlayer)
				{
					NewPlayerMsg type;
					memcpy(&type, recvbuf, sizeof(NewPlayerMsg));
					if (type.msg.head.id == client->client_id)
					{
						std::cout << "Me again. \n";
					}
					else if(this->enemy->client_id == -1)
					{
						std::cout << "Opponent joined. \n";
						this->enemy->client_id = type.msg.head.id;
					}
					else
					{
						std::cout << "Max players already in game. \n";
					}

				}
				if (msgType.type == PlayerLeave)
				{	
					client->seq_nr = msghead.seq_no;
					std::cout << "Player left\n";
				}
				if (msgType.type == NewPlayerPosition)
				{
					NewPlayerPositionMsg type;
					memcpy(&type, recvbuf, sizeof(NewPlayerPositionMsg));
					if (msgType.head.id == client->client_id)
					{
						client->pos = type.pos;
						std::cout << "New this player position: x:" << client->pos.x << " y: " << client->pos.y << "\n";
					}
					else if (msgType.head.id == enemy->client_id)
					{
						enemy->pos = type.pos;
						std::cout << "New enemy player position: x:" << enemy->pos.x << " y: " << enemy->pos.y << "\n";
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

void NetworkListener::join()
{
	JoinMsg join;
	char sendbuf[sizeof(join)];

	//filling join message
	join.head.id = 0;
	join.head.seq_no = 0;
	join.head.length = sizeof(join);
	join.head.type = Join;

	join.desc = Human;
	join.form = Cube;

	//copying join message to sendbuf
	memcpy((void*)sendbuf, (void*)&join, sizeof(join));

	//Encrypt
	rc4Out->Encrypt(sendbuf, sizeof(sendbuf));

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, sizeof(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return;
	}

	printf("Bytes Sent: %ld\n", iResult);
}

