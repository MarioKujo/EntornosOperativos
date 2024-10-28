#pragma comment(lib, "ws2_32.lib") //add WinSock2 library

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <assert.h>
#include "..\TCPMailboxLibEmpty\TCPMailboxLibEmpty.h"
#include <iostream>
using namespace std;

int serverFun(DataPacket clientPacket, PDataPacket storage, SOCKET s, string prefix);
int serverWrite(PDataPacket storage, SOCKET s, string prefix);
int serverRead(PDataPacket storage, SOCKET s, string prefix);

int main()
{
	std::cout << "Server: starting..." << std::endl;
	//required intialization of WinSock 2 library, it writes some data om wsaData to check everything is ok
	int result;
	WSAData wsaData;
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	assert(result == NO_ERROR);
	std::cout << "Server: WinSock started correctly" << std::endl;

	//now we create a socket that uses IP (AF_INET) with TCP (SOCK_STREAM and IPPROTO_TCP) 
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//assert(s != INVALID_SOCKET);
	if (result == INVALID_SOCKET) {
		treatErrorExit("Server: Socket creation error: ", s, -1);
	}
	std::cout << "Server: socket created" << std::endl;

	//now we specify the other machine we want to send messages to
	sockaddr_in my_addr;
	PCSTR address = "127.0.0.1";
	if (!inet_pton(AF_INET, address, &(my_addr.sin_addr.s_addr))) { // Replace with your desired IP address
		treatErrorExit("Server: error converting IP in string to binary: ", s, -1);
	}
	my_addr.sin_family = AF_INET;
	//store bytes in network format == big-endian
	my_addr.sin_port = htons(4000); // Replace with your desired port or 0 so that the OS chooses one that is available

	//in the server we need to bind in order to be able to listen through the socket
	result = bind(s, (sockaddr*)&my_addr, sizeof(my_addr));
	//assert(result != SOCKET_ERROR);
	if (result == SOCKET_ERROR) {
		treatErrorExit("Server: bind error", s, -1);
	}
	std::cout << "Server: socket bound to address: " << address << " port: " << my_addr.sin_port << std::endl;

	//listen for incoming connections
	//int listen_ret;
	if (listen(s, 1) == SOCKET_ERROR) {
		//std::cout << "Server: listen(): Error listening on socket: " << WSAGetLastError() << std::endl;
		treatErrorExit("Server: listen(): Error listening on socket: ", s, -1);
	}

	//main server loop: while there are incoming connections
	bool serve = true;
	PDataPacket packet = new DataPacket;
	PDataPacket storage = new DataPacket();
	string prefix = "Server: ";
	SOCKET acceptSocket; //this is a new socket specific for the TCP connection with a client
	// Accept incoming connections
	while (serve && (acceptSocket = accept(s, nullptr, nullptr))) {
		std::cout << "Server accepted connection" << std::endl;
		std::cout << "Server ready to recv" << std::endl;

		//while a client sends msgs we keep talking to them
		int rbyteCount;
		// receive the packet and write its data to the packet struct
		while (rbyteCount = recvMsg(acceptSocket, packet, prefix)) {
			if (rbyteCount < 0) {//if any error occurrs, try to recv again
				continue;
			}
			DataPacket clientPacket = (DataPacket)*packet;
			//cout << "ClientPacket: " << clientPacket << endl;

			//TODO:do something calling serverFun
			serverFun(clientPacket, storage, acceptSocket, prefix);
		}//if the client stops sending msgs, its socket will be closed, and acceptSocket will also be closed
	}
	// Check for successful connection
	if (acceptSocket == INVALID_SOCKET) {
		treatError("Server: accept failed: ", acceptSocket);
	}
	std::cout << "Server: cleaning up and returning" << endl;
	// cleanup
	closesocket(s);
	WSACleanup();
}

//TODO:prints clientPacket, then checks if clientPacket.msg is "Write" or "Read" and uses the following functions accordingly
int serverFun(DataPacket clientPacket, PDataPacket storage, SOCKET acceptSocket, string prefix) {
	cout << "ClientPacket:" << clientPacket << endl;
	if (strcmp(clientPacket.msg, "Write") == 0) {
		return serverWrite(storage, acceptSocket, prefix);
	}
	else if (strcmp(clientPacket.msg, "Read") == 0) {
		cout << "Server reading..." << endl;
		return serverRead(storage, acceptSocket, prefix);
	}
	return 0;
}
//TODO:recv next msg and stores it, also prints it
int serverWrite(PDataPacket storage, SOCKET acceptSocket, string prefix) {
	int result = recvMsg(acceptSocket, storage, prefix);
	if (result != SOCKET_ERROR)
	{
		cout << "Stored message: " << *storage << endl;
	}
	return result;
}
//TODO:sends the storage to the client and prints it
int serverRead(PDataPacket storage, SOCKET acceptSocket, string prefix) {
	int result = sendMsg(acceptSocket, storage, prefix);
	if (result != SOCKET_ERROR)
	{
		cout << "Stored message: " << *storage << endl;
	}
	return result;
}
