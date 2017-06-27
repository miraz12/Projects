#include "connections.h"
#include <stdio.h>
#include <iostream>
#ifdef __linux__ 
#include <string.h>
#include <unistd.h>

#endif
#include "server.h"
#include <vector>


Connections::Connections(SOCKET sockfd, struct sockaddr_storage &their_addr, socklen_t sin_size, Server* server)
{
	printf("Client started\n");
	this->connectSocket = sockfd;
	this->their_addr = their_addr;
	this->sin_size = sin_size;
	this->server = server;
	this->id = 0;
	this->lastSeqNo = 0;

	keyIn = "Client";
	keyOut = "Server";

	rc4In = new RC4(keyIn, 6);
	rc4Out = new RC4(keyOut, 6);

	this->receiverThread = std::thread(&Connections::Receiver, this);
}

Connections::~Connections()
{
}

void Connections::Receiver()
{
	int iResult = 0;
	printf("Starting recive");
	char* recvbuf = new char[DEFAULT_BUFLEN];

	// Receive data until connection is closed
	do {
		memset(recvbuf, 0, DEFAULT_BUFLEN);
		iResult = recv(connectSocket, recvbuf, DEFAULT_BUFLEN, 0);

		if (iResult > 0)
		{
			//decrypt
			rc4In->Decrypt(recvbuf, iResult);

			printf("Bytes received: %d\n", iResult);
			MsgHead msghead;
			memset(&msghead, 0, sizeof(MsgHead));
			memcpy(&msghead, recvbuf, sizeof(MsgHead));

			std::cout << "id " << msghead.id << " length " << msghead.length << " sekvensnummer " << msghead.seq_no << std::endl;
			switch (msghead.type)
			{
			case Join:
			{
				printf("Recived Join message.\n");
				JoinMsg joinMsg;
				memcpy(&joinMsg, recvbuf, sizeof(JoinMsg));
				OnJoin(joinMsg);
				break;
			}
			case Leave:
			{
				printf("Recived Leave message. Player: %d\n", this->id);

				PlayerLeaveMsg playerLeave;
				playerLeave.msg.head.length = sizeof(PlayerLeaveMsg);;
				playerLeave.msg.head.id = this->id;
				playerLeave.msg.head.type = Change;
				playerLeave.msg.type = PlayerLeave;

				for (auto i = server->getConnections().begin(); i < server->getConnections().end(); i++) {
					(*i)->lastSeqNo++;
					playerLeave.msg.head.seq_no = (*i)->lastSeqNo;
					(*i)->SendMsg((unsigned char*)&playerLeave, playerLeave.msg.head.length);
				}
				iResult = 0;
				break;
			}
			case Event:
			{
				printf("Recived Event message.\n");
				EventMsg eventMsg;
				memcpy(&eventMsg, recvbuf, sizeof(EventMsg));

				if (eventMsg.type == Move)
				{
					printf("Move msg.\n");
					MoveEvent moveMsg;
					memcpy(&moveMsg, recvbuf, sizeof(MoveEvent));
					//If not out of bounds
					if (moveMsg.pos.x <= 100 && moveMsg.pos.x >= -100 && moveMsg.pos.y <= 100 && moveMsg.pos.y >= -100)
					{
						for (auto i = server->getConnections().begin(); i < server->getConnections().end(); i++) {
							(*i)->lastSeqNo++;
							NewPlayerPositionMsg playerPos;

							playerPos.pos = moveMsg.pos;
							playerPos.dir = moveMsg.dir;
							playerPos.msg.head.id = this->id;
							playerPos.msg.head.type = Change;
							playerPos.msg.type = NewPlayerPosition;
							playerPos.msg.head.length = sizeof(NewPlayerPositionMsg);
							playerPos.msg.head.seq_no = (*i)->lastSeqNo;
							printf("sending move obj %i to client %i\n", this->id, (*i)->id);
							(*i)->SendMsg((unsigned char*)&playerPos, sizeof(NewPlayerPositionMsg));
						}
					}
					else
					{
						//Do nothing. The client wont get a move msg back and thus will stand still. 
					}
				}
				break;
			}
			case TextMessage:
			{
				printf("Recived TextMessage message.\n");
				break;
			}
			}
		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
#ifdef _WIN32
			printf("recv failed: %d\n", WSAGetLastError());
#else
			printf("recv failed\n");
#endif
	
		} while (iResult > 0);

		server->RemoveConnection(connectSocket);
}

void Connections::SendMsg(unsigned char* packet, int packetLen)
{
	int iResult = 0;
	//Encrypt
	this->rc4Out->Encrypt((char*)packet, packetLen);

	iResult = send(this->connectSocket, (char*)packet, packetLen, 0); 
	if (iResult == -1)
	{
		printf("Send failed\n");
		return;
	}

	printf("Bytes Sent: %d\n", iResult);
}

void Connections::OnJoin(JoinMsg &joinMsg)
{
	const std::vector<Connections*> listCopy = server->getConnections();
	char sendbuf[sizeof(joinMsg)];
	int iResult = 0;

	//Assign this clients name and id
	this->id = this->connectSocket;
	this->name = joinMsg.name;

	this->lastSeqNo++;
	//Fill head
	joinMsg.head.id = this->id;
	joinMsg.head.seq_no = this->lastSeqNo;

	JoinMsg joinMsgSend;
	joinMsgSend.head.id = this->id;
	joinMsgSend.head.seq_no = this->lastSeqNo;
	joinMsgSend.head.length = sizeof(JoinMsg);
	joinMsgSend.head.type = Join;

	memcpy((void*)sendbuf, (void*)&joinMsgSend, sizeof(JoinMsg));

	// send packet
	this->SendMsg((unsigned char*)sendbuf, sizeof(JoinMsg));
	printf("sending JoinMsg %i to client %i\n", this->id, this->id);

	const unsigned int totalLength = sizeof(NewPlayerMsg);
	char* sendBuffer = new char[totalLength];
	for (auto i = server->getConnections().begin(); i < server->getConnections().end(); i++) 
	{
		(*i)->lastSeqNo++;
		NewPlayerMsg playerMsg;
		playerMsg.msg.head.length = totalLength;
		playerMsg.msg.head.seq_no = this->lastSeqNo;
		playerMsg.msg.head.id = this->id;
		playerMsg.msg.head.type = Change;
		playerMsg.msg.type = NewPlayer;
		playerMsg.msg.head.seq_no = (*i)->lastSeqNo;
		memcpy(sendBuffer, &playerMsg, totalLength);

		printf("sending JoinMsg %i to client %i\n", this->id, (*i)->id);
		(*i)->SendMsg((unsigned char*)sendBuffer, totalLength);
	}
#ifdef _WIN32
	Sleep(10);
#endif // _WIN32
#ifdef __linux__
	sleep(1);
#endif
	// write add player messages to single client
	const unsigned int totalLength2 = sizeof(NewPlayerMsg);
	char* sendBuffer2 = new char[totalLength2];
	for (auto i = server->getConnections().begin(); i < server->getConnections().end(); i++) 
	{
		// dont add this
		if ((*i) == this)
			continue;
		NewPlayerMsg msg1;
		msg1.msg.head.type = Change;
		msg1.msg.type = NewPlayer;
		msg1.msg.head.length = totalLength2;
		this->lastSeqNo++;
		msg1.msg.head.seq_no = this->lastSeqNo;
		msg1.msg.head.id = (*i)->id;
		memcpy(sendBuffer2, &msg1, totalLength2);
		this->SendMsg((unsigned char*)sendBuffer2, totalLength2);
	}
}
