#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <assert.h>
#include "..\TCPMailboxLibEmpty\TCPMailboxLibEmpty.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <format>

#define MAX_MSGS 7
using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "UDPClient usage: .\\UDPClient <id>" << endl;
        ExitProcess(-1);
    }
    int client = atoi(argv[1]);
    std::cout << format("Client[{}]: starting...", client) << std::endl;
    //required intialization of WinSock 2 library, it writes some data om wsaData to check everything is ok
    int result;
    WSAData wsaData;
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    assert(result == NO_ERROR);
    std::cout << "Client: WinSock started correctly" << std::endl;

    //now we create a socket that uses IP (AF_INET) with TCP (SOCK_STREAM and IPPROTO_TCP) 
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //assert(s != INVALID_SOCKET);
    if (result == INVALID_SOCKET) {
        treatErrorExit("Client: Socket creation error: ", s, -1);
    }
    std::cout << "Client: socket created" << std::endl;
    //now we specify the other machine we want to send messages to
    sockaddr_in addr;
    //addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //deprecated
    PCSTR address = "127.0.0.1";
    if (!inet_pton(AF_INET, address, &(addr.sin_addr.s_addr))) { // Replace with your desired IP address
        treatErrorExit("Client: error converting IP in string to binary\n", s, -1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4000);

    //now we connect to the desired address
    result = connect(s, (sockaddr*)&addr, sizeof(addr));
    //assert(result != SOCKET_ERROR);
    if (result == SOCKET_ERROR) {
        treatErrorExit("Client: connect error: ", s, -1);
    }
    std::cout << "Client: socket connected to address: " << address << " port: " << addr.sin_port << std::endl;

    string write = "Write";
    string read = "Read";
    std::string msgs[MAX_MSGS] = { write, "Hola", read, read, write, "Adios", read };
    for (int i = 0; i < MAX_MSGS; i++) {
        std::cout << "Client ready to send: " << msgs[i] << std::endl;
        //TODO:create packet and allocate for response
        
        //cout << "new packet " << *packet << endl;
        
        //TODO:when reading send Read and recv the msg from server printing it
        //else just send the Write and the msg to write in 2 consecutive msgs
        
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
