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

#include <thread>
#include "structs.h"
#include "RC4.h"
class Server;

class Connections
{
public:
	Connections(SOCKET sockfd, struct sockaddr_storage &their_addr, socklen_t sin_size, Server* server);
	~Connections();
	void Receiver();
	void SendMsg(unsigned char* packet, int packet_len);
	void OnJoin(JoinMsg &joinMsg);

	int getConnectSocket() const { return connectSocket; }

	int& getId() { return id; }
	void setId(int val) { id = val; }
private:
	Server* server;
	SOCKET connectSocket;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;

	int id;
	unsigned lastSeqNo;
	char* name;

	RC4* rc4In;
	RC4* rc4Out;
	const char* keyIn;
	const char* keyOut;

	std::thread receiverThread;
};

