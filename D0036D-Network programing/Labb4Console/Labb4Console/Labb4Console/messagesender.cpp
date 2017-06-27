#include "stdafx.h"
#include "messagesender.h"
#include <iostream>
#include <string>
#include <sstream>


MessageSender::MessageSender()
{
}

MessageSender::MessageSender(SOCKET connectsocket, Client* inClient) : client(inClient)
{
	this->ConnectSocket = connectsocket;
}


MessageSender::~MessageSender()
{
}

void MessageSender::join()
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

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, sizeof(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return;
	}

	printf("Bytes Sent: %ld\n", iResult);

	this->InputListener();
}

void MessageSender::sendMove(std::string dir)
{
	MoveEvent move;
	
	char sendbuf[sizeof(move)];

	move.event.type = Move;
	//filling move message
	move.event.head.length = sizeof(move);
	move.event.head.seq_no = client->seq_nr++;
	move.event.head.id = client->client_id;
	move.event.head.type = Event;

	if (dir == "left")
	{
		Coordinate newPos;
		newPos.x = client->pos.x - 1;
		newPos.y = client->pos.y;
		move.pos = newPos;
		move.dir = newPos;
	}
	else if (dir == "right")
	{
		Coordinate newPos;
		newPos.x = client->pos.x + 1;
		newPos.y = client->pos.y;
		move.pos = newPos;
		move.dir = newPos;
	}

	//copying move message to sendbuf
	memcpy((void*)sendbuf, (void*)&move, sizeof(move));

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

void MessageSender::sendLeave()
{
	LeaveMsg leave;

	char sendbuf[sizeof(leave)];

	//filling leave message
	leave.head.seq_no = client->seq_nr++;
	leave.head.id = client->client_id;
	leave.head.length = sizeof(leave);
	leave.head.type = Leave;


	//copying leave message to sendbuf
	memcpy((void*)sendbuf, (void*)&leave, sizeof(leave));

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

void MessageSender::InputListener()
{
	std::string input;

	while (true)
	{
		std::cin >> input;

		if (input == "close")
		{
			break;
		}
		if (input == "left")
		{
			this->sendMove("left");
		}
		if (input == "right")
		{
			this->sendMove("right");
		}
		if (input == "leave")
		{
			this->sendLeave();
		}
	}
	

}

