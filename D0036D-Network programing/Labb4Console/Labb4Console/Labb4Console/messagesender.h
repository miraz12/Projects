#pragma once
#include "structs.h"
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include "structs.h"
#include <string>

class MessageSender
{
public:
	MessageSender();
	MessageSender(SOCKET connectsocket, Client* inClient);
	~MessageSender();
	void join();


private:
	void sendMove(std::string dir);
	void sendLeave();
	void sendTextMessage();

	void InputListener();


	SOCKET ConnectSocket;
	int iResult;

	Client* client;
};

