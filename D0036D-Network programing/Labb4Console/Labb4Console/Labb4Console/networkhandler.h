#pragma once
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include "structs.h"

///Singleton network handler handels all basic network functions
class NetworkHandler
{
public:
	NetworkHandler(Client* inClient);
	~NetworkHandler();

	void listener();

	//Singelton
	//static NetworkHandler* getInstance();

	int Result() const { return iResult; }
	void Result(int val) { iResult = val; }

	SOCKET ConnectSocket;
private:
	
	int iResult;
	Client* client1;
	Client* client2;


};

