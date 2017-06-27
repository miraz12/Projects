#include "server.h"
#include <stdio.h>
#include <string>
#ifdef __linux__ 
#include <string.h>
#endif
#include <cstring>

#include "connections.h"


Server::Server()
{

#ifdef _WIN32
	// If Windows
	WSADATA wsaData;   // if this doesn't work then try this instead //WSAData wsaData;
	// MAKEWORD(1,1) for Winsock 1.1, MAKEWORD(2,0) for Winsock 2.0:
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		fprintf(stderr, "WSAStartup failed.\n");
	}
#endif

	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_INET;     // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

	int iResult = 0;
	if ((iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(iResult));
		exit(7);
	}

	if ((connectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == -1) {
		printf("Socket error");
		return;
	}

	if (bind(connectSocket, result->ai_addr, result->ai_addrlen) == -1) {
		printf("Bind error");
	}

	struct sockaddr_in sin;
	int len = sizeof(sin);
	if (getsockname(connectSocket, (struct sockaddr *)&sin, (socklen_t*)&len) == 0 && sin.sin_family == AF_INET && len == sizeof(sin))
	{
		unsigned int local_port = ntohs(sin.sin_port);
		printf("Server started on port: %i\n", local_port);
	}
	else
		printf("Couldn't get local port\n");

	if (result == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(connectSocket, SOMAXCONN) == -1) {
		fprintf(stderr, "bind error: %s\n", gai_strerror(errno));
		exit(4);
	}

	printf("Waiting for clients...\n");

	this->Accept();
}

Server::~Server()
{
}

void Server::Accept()
{
	while (true)
	{
		struct sockaddr_storage their_addr;
		socklen_t sin_size = sizeof(their_addr);
		SOCKET client = 0;

		//printf("server sockfd is %i\n", connectSocket);

		client = accept(connectSocket, (struct sockaddr *)&their_addr, &sin_size);
		printf("Incoming client connection...\n");
		if (client == -1) {
			fprintf(stderr, "accept error: %s\n", gai_strerror(errno));
			system("PAUSE");
			exit(6);
		}

		printf("New client on socket: %d\n", client);
		connections.push_back(new Connections(client, their_addr, sin_size, this));
	}
}

void Server::RemoveConnection(SOCKET conSock)
{
	for (auto i = connections.begin(); i < connections.end();) 
	{
		if ((*i)->getConnectSocket() == conSock) 
		{
			printf("Removing\n");
			if (i == connections.end()) 
			{
				connections.erase(i);
			}
			else 
			{
				i = connections.erase(i);
			}
		}
		else
			i++;
	}
}
