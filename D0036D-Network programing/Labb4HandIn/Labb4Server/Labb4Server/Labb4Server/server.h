#pragma once

#ifdef __linux__ 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
typedef int SOCKET;
#define  DEFAULT_PORT "49152"

#elif _WIN32
#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#define DEFAULT_PORT "49152"
#endif

#define DEFAULT_BUFLEN 1024
class Connections;
#include <vector>
#include "RC4.h"


class Server
{
public:
	///Sets upp winsock and then starts Accept loop
	Server();
	~Server();
	///Listens for incoming client connections and creates a new Connection() thread for it.
	void Accept();
	///Cleanup
	void RemoveConnection(SOCKET conSock);

	///Getters & Setters
	std::vector<Connections*>& getConnections(){ return this->connections; }
private:
	SOCKET connectSocket;
	struct addrinfo hints;
	struct addrinfo* result;
	std::vector<Connections*> connections;

};

