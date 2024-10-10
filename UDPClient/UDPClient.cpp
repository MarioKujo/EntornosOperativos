#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <assert.h>
#include "UDPLib.h"
#include <iostream>
#include <string>
#include <Windows.h>

#define MAX_MSGS 4
using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "UDPClient usage: .\\UDPClient <id>" << endl;
        ExitProcess(-1);
    }
    int client = atoi(argv[1]);
    std::cout << "Client: starting..." << std::endl;
    //required intialization of WinSock 2 library, it writes some data om wsaData to check everything is ok
    int result;
    WSAData wsaData;
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    assert(result == NO_ERROR);
    std::cout << "Client: WinSock started correctly" << std::endl;

    //now we create a socket that uses IP (AF_INET) with UDP (SOCK_DGRAM and IPPROTO_UDP) 
    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    //assert(s != INVALID_SOCKET);
    if (s == INVALID_SOCKET) {
        treatErrorExit("Client: Socket creation error: ", s, -1);
    }
    std::cout << "Client: socket created" << std::endl;
    //now we specify the other machine we want to send messages to
    sockaddr_in server_addr;
    PCSTR address = "127.0.0.1";
    if (!inet_pton(AF_INET, address, &(server_addr.sin_addr.s_addr))) { // Replace with your desired IP address
        printf("error converting IP in string to binary\n");
        return -1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4000);

    //now we connect to the desired address
    /*result = connect(s, (sockaddr*)&addr, sizeof(addr));
    assert(result != SOCKET_ERROR);
    std::cout << "Client: socket connected to address: " << address << " port: " << addr.sin_port << std::endl;
    */
    std::string msgs[MAX_MSGS] = { "hola", "que tal?", "adios", "Close_Server"};
    for (int i = 0; i < MAX_MSGS; i++) {
        std::cout << "Client ready to send: " << msgs[i] << std::endl;
        //now we initialize the data we want to send
        DataPacket packet;
        packet.client_id = client;
        packet.sequence = i;
        //deep copy of string into the struct so that data reaches the server, NEVER send a pointer / shallow copy
        memset(packet.msg, 0, MSG_SIZE); //clear all the memory in packet.msg before copying a new msg
        msgs[i].copy(packet.msg, msgs[i].size(), 0); 
        //Sleep(100); //sleep for a second just so that there is time for communication and printing
        //now we just send the data through the socket
        //we make the casting to char* because it expects data as just chars, last param 0 is for flags that we don't need
        result = sendto(s, (char*)&packet, sizeof(DataPacket), 0, (SOCKADDR*)&server_addr, sizeof(SOCKADDR));
        assert(result != SOCKET_ERROR);

        cout << "Client: succesfully sent msg" << endl;
        //receive response from Server
        DataPacket response;
        int fromlen = sizeof(SOCKADDR);
        //recvfrom addr is ALWAYS an out param, but in this case we overwrite it with the same IP:PORT of the server 
        result = recvfrom(s, (char*)&response, sizeof(DataPacket), 0,(SOCKADDR*)&server_addr, &fromlen);
        assert(result != SOCKET_ERROR);
        
        cout << "Client: succesfully received response: " << response << endl;
    }

    std::cout << "Client finishing..." << std::endl;
    int iResult = closesocket(s);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"closesocket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    WSACleanup();
    return 0;
}

