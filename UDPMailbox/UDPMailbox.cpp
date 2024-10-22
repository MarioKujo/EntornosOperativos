#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <assert.h>
#include "..\UDPMailboxLib\UDPMailboxLib.h"
#include <iostream>
#include <string>
#include <Windows.h>

#define MAX_MSGS 7
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
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }
    string write = "Write";
    string read = "Read";
    std::string msgs[MAX_MSGS] = { write, "Hola", read, read, write, "Adios", read };
    for (int i = 0; i < MAX_MSGS; i++) {
        std::cout << "Client ready to send: " << msgs[i] << std::endl;
        //TODO:
        //create packet and allocate for response
        DataPacket packet(client, i, msgs[i]);
        DataPacket response;
        //when reading send Read and recv the msg from server printing it
        if (msgs[i] == "Read") {
            if (i - 1 >= 0 && msgs[i - 1] != "Read")
            {
                sendtorecvfromMsg(s, &server_addr, &packet, &response, "Client");
                if (response.msg != NULL) {
                    cout << "Response from server: " << response << endl;
                }
            }
        }
        //else just send the Write and the msg-to-write in 2 consecutive msgs
        else if (msgs[i] == "Write") {
            sendtoMsg(s, &server_addr, &packet, "Client");
            if (i + 1 < MAX_MSGS) {
                DataPacket tempPacket(client, i + 1, msgs[i + 1]);
                sendtoMsg(s, &server_addr, &tempPacket, "Client");
                i++;
            }
        }
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



