#pragma comment(lib, "ws2_32.lib") //add WinSock2 library

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <assert.h>
#include "..\UDPMailboxLib\UDPMailboxLib.h"
#include <iostream>
using namespace std;

int serverFun(DataPacket clientPacket, PDataPacket storage, SOCKET s, sockaddr_in client_addr);
int serverWrite(PDataPacket storage, SOCKET s, sockaddr_in client_addr);
int serverRead(PDataPacket storage, SOCKET s, sockaddr_in client_addr);

int main()
{
    std::cout << "Server: starting..." << std::endl;
    //required intialization of WinSock 2 library, it writes some data om wsaData to check everything is ok
    int result;
    WSAData wsaData;
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    assert(result == NO_ERROR);

    std::cout << "Server: WinSock started correctly" << std::endl;
    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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
    my_addr.sin_port = htons(4000); //store bytes in network format big-endian

    //in the server we need to bind in order to be able to recvfrom through the socket
    result = bind(s, (sockaddr*)&my_addr, sizeof(my_addr));
    if (result == SOCKET_ERROR) {
        treatErrorExit("Server: bind error", s, -1);
    }

    PDataPacket packet = new DataPacket();
    std::cout << "Server: socket bound to address: " << address << " port: " << my_addr.sin_port << std::endl;

    bool serve = true;
    PDataPacket storage = new DataPacket();
    while (serve) {
        std::cout << "Server ready to recv" << std::endl;
        //TODO:recv msg, then cast it to DataPacket and call serverFun
        sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        int result = recvfromMsg(s, &client_addr, packet, "Server");
        if (result != SOCKET_ERROR) {
            serverFun(*packet, storage, s, client_addr);//Esto es solo para que se espere a tener conexión de un cliente antes de empezar a intentar enviar o recibir mensajes
        }
    }
    std::cout << "Server: cleaning up and returning" << endl;
    // cleanup
    closesocket(s);
    WSACleanup();

}

//TODO:prints clientPacket, then checks if clientPacket.msg is "Write" or "Read" and uses the following functions accordingly
int serverFun(DataPacket clientPacket, PDataPacket storage, SOCKET s, sockaddr_in client_addr) {
    cout << "ClientPacket: " << clientPacket << endl;
    if (strcmp(clientPacket.msg, "Write") == 0) {
        return serverWrite(storage, s, client_addr);
    }
    else if (strcmp(clientPacket.msg, "Read") == 0) {
        return serverRead(storage, s, client_addr);
    }    
    return 0;
}

//TODO:recvfrom next msg and stores it, also prints it
int serverWrite(PDataPacket storage, SOCKET s, sockaddr_in client_addr) {
    int result = recvfromMsg(s, &client_addr, storage, "Server");
    if(result != SOCKET_ERROR)
    {
        cout << "Stored message: " << *storage << endl;
    }
    return result;
}
//TODO:sends the storage to the client and prints it
int serverRead(PDataPacket storage, SOCKET s, sockaddr_in client_addr) {
    int result = sendtoMsg(s, &client_addr, storage, "Server");
    if (result != SOCKET_ERROR)
    {
        cout << "Sent to client: " << *storage << endl;
    }
    return 0;
}