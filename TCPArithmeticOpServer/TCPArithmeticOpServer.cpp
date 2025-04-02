#pragma comment(lib, "ws2_32.lib") //add WinSock2 library

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <assert.h>
#include "../TCPArithmeticOpLib/TCPArithmeticOpLib.h"
#include <iostream>
using namespace std;

int serverFun(SOCKET acceptSocket, PDataPacket clientPacket);


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

            //TODO:do something calling serverFun
            serverFun(acceptSocket, packet);
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

//TODO: makes operation with op1 and op2 storing the result in res, all of them fields of clientPacket
int serverFun(SOCKET acceptSocket, PDataPacket clientPacket) {
    long long result = 0;

    switch (clientPacket->operation) {
    case SUM:
        result = clientPacket->op1 + clientPacket->op2;
        break;
    case DIFF:
        result = clientPacket->op1 - clientPacket->op2;
        break;
    case PROD:
        result = clientPacket->op1 * clientPacket->op2;
        break;
    case DIV:
        if (clientPacket->op2 != 0) {
            result = clientPacket->op1 / clientPacket->op2;
        }
        else {
            std::cout << "Division by zero!" << std::endl;
            return -1; // Manejo de error
        }
        break;
    case POWER:
        result = static_cast<long long>(pow(clientPacket->op1, clientPacket->op2));
        break;
    default:
        std::cout << "Unknown operation!" << std::endl;
        return -1; // Manejo de error
    }

    // Guardar el resultado en el paquete
    clientPacket->res = result;

    // Enviar de vuelta la respuesta
    std::cout << "Sent result: " << clientPacket->res << " for operation: " << *clientPacket << std::endl;
    sendMsg(acceptSocket, clientPacket, "Server");
    return 0;
}