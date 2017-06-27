#pragma once
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

class MessageListener
{
public:
	MessageListener();
	MessageListener(SOCKET sock);
	~MessageListener();
	void listener(SOCKET sock);

private:

	int iResult;

};

