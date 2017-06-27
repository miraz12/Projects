// Labb4Console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "networkhandler.h"
#include <thread>
#include <iostream>
#include "messagelistener.h"
#include "messagesender.h"
#include "structs.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Client* c = new Client;
	unsigned int* seq_nr = 0;
	int* client_id = 0;

	NetworkHandler* net = new NetworkHandler(c);


	std::thread t1(&NetworkHandler::listener, net);
	std::thread t2(&MessageSender::join, MessageSender(net->ConnectSocket, c));

	t1.join();
	t2.join();

	std::cout << "Threads done! \n";

	return 0;
}

