#pragma comment(lib, "ws2_32.lib") //add WinSock2 library

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <assert.h>
#include "..\UDPArithmeticOpLibEmpty\UDPArithmeticOpLib.h"
#include <iostream>
#include <math.h>
using namespace std;

int serverFun(PDataPacket clientPacket, SOCKET s, sockaddr_in client_addr);

int main()
{
    string prefix = "Server: ";
    std::cout << "Server: starting..." << std::endl;
    //required intialization of WinSock 2 library, it writes some data om wsaData to check everything is ok
    int result;
    WSAData wsaData;
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    assert(result == NO_ERROR);

    std::cout << "Server: WinSock started correctly" << std::endl;

    //now we create a socket that uses IP (AF_INET) with UDP (SOCK_DGRAM and IPPROTO_UDP)
    
    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    //assert(s != INVALID_SOCKET);
    if (result == INVALID_SOCKET) {
        treatErrorExit("Server: Socket creation error: ", s, -1);
    }
    std::cout << prefix << "socket created" << std::endl;

    //now we specify the other machine we want to send messages to
    sockaddr_in my_addr;
    PCSTR address = "127.0.0.1";
    if (!inet_pton(AF_INET, address, &(my_addr.sin_addr.s_addr))) { // Replace with your desired IP address
        treatErrorExit("Server: error converting IP in string to binary: ", s, -1);
    }
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(4000); //store bytes in network format big-endian

    //in the server we need to bind in order to be able to recvfrom through the socket
    result = bind(s, (sockaddr*)&my_addr, sizeof(my_addr));
    //assert(result != SOCKET_ERROR);
    if (result == SOCKET_ERROR) {
        treatErrorExit("Server: bind error", s, -1);
    }

    PDataPacket packet = new DataPacket();
    std::cout << "Server: socket bound to address: " << address << " port: " << my_addr.sin_port << std::endl;

    while (true) {
        std::cout << "Server ready to recv" << std::endl;
        //TODO:recv msg, then cast it to DataPacket and call serverFun
        sockaddr_in client_addr;
        int addr_len = sizeof(client_addr);
        result = recvfromMsg(s, &client_addr, packet, "Server");
        if (result == SOCKET_ERROR) {
            continue;
        }
        //TODO:do something with serverFun
        serverFun(packet, s, client_addr);
    }
    std::cout << "Server: cleaning up and returning" << endl;
    // cleanup
    closesocket(s);
    WSACleanup();

}
//makes operation with op1 and op2 storing the result in res, all of them fields of clientPacket
int serverFun(PDataPacket clientPacket, SOCKET s, sockaddr_in client_addr) {
    //TODO
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
    sendtoMsg(s, &client_addr, clientPacket, "Server");
    std::cout << "Sent result: " << clientPacket->res << " for operation: " << *clientPacket << std::endl;

    return 0;
}
