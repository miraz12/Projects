#include "stdafx.h"
#include "messagelistener.h"
#include <iostream>
#include "structs.h"
#include <thread>



#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define DEFAULT_PORT "49152"
#define DEFAULT_BUFLEN 512
MessageListener::MessageListener()
{
}

MessageListener::MessageListener(SOCKET sock)
{
	listener(sock);
}

MessageListener::~MessageListener()
{

}

void MessageListener::listener(SOCKET sock)
{
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN];

	// Receive data until the server closes the connection
	do {
		iResult = recv(sock, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{

			printf("Bytes received: %d\n", iResult);
			MsgHead* msghead;
			MsgType* msgType;
			msghead = (MsgHead*)recvbuf;
			msgType = (MsgType*)recvbuf;

			std::cout << "id " << msghead->id << " length " << msghead->length << " sekvensnummer " << msghead->seq_no << std::endl;

			if (sizeof(msgType) == sizeof(NewPlayerMsg))
			{
				NewPlayerMsg* changeType;
				changeType = (NewPlayerMsg*)recvbuf;
				std::cout << "new player message\n";

			}
			if (msghead->length == sizeof(NewPlayerPositionMsg))
			{
				NewPlayerPositionMsg* changeType;
				changeType = (NewPlayerPositionMsg*)recvbuf;
				std::cout << "new player pos message\n";
			}
			if (msghead->length == sizeof(PlayerLeaveMsg))
			{
				PlayerLeaveMsg* changeType;
				changeType = (PlayerLeaveMsg*)recvbuf;
				std::cout << "player leave message\n";

			}

		}
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (iResult > 0);

}
