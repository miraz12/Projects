#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "structs.h"
#include "RC4.h"


class NetworkListener
{
public:
	///Def con
	NetworkListener();
	///Sets clients, sockets and encryption from incoming values.
	NetworkListener(Client* inClient, Client* inEnemy, SOCKET sock, RC4* rc4in, RC4* rc4out);
	///Def de
	~NetworkListener();

	///Starts a loop that listens for incoming messages and handles them.
	void listener();
	///Creates ands sends join message to server
	void join();

private:
	SOCKET ConnectSocket;
	Client* client;
	Client* enemy;
	int iResult;

	RC4* rc4In;
	RC4* rc4Out;


};

