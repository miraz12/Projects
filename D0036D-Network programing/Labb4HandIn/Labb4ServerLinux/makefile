#MyServer : main.o Client.o Object.o RC4.o Server.o
#	gcc -std=c++0x -o MyServer main.o Client.o Object.o RC4.o Server.o
#
#main.o : main.cpp server.h
#	gcc -std=c++0x -c main.cpp
#
#Client.o : connections.cpp RC4.h connections.h
#	gcc -std=c++0x -c connections.cpp
#
#RC4.o : RC4.cpp RC4.h
#	gcc -std=c++0x -c RC4.cpp
#
#Server.o : server.cpp server.h 
#	gcc -std=c++0x -c server.cpp

#Clean :
#	rm MyServer

MyServer : 
	g++ -pthread -std=c++0x -o MyServerShaggy main.cpp connections.cpp RC4.cpp server.cpp
