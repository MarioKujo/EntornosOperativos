#pragma comment(lib, "ws2_32.lib") //add WinSock2 library

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <assert.h>
#include "UDPLib.h"
#include <iostream>
using namespace std;

int main()
{
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
    if (s == INVALID_SOCKET) {
        treatErrorExit("Server: Socket creation error: ", -1, s);
    }
    std::cout << "Server: socket created" << std::endl;

    //now we specify the other machine we want to send messages to
    sockaddr_in my_addr;
    PCSTR address = "127.0.0.1";
    if (!inet_pton(AF_INET, address, &(my_addr.sin_addr.s_addr))) { // Replace with your desired IP address
        treatErrorExit("Server: error converting IP in string to binary: ", -1, s);
    }
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(4000); //store bytes in network format big-endian

    //in the server we need to bind in order to be able to recvfrom through the socket
    result = bind(s, (sockaddr*)&my_addr, sizeof(my_addr));
    //assert(result != SOCKET_ERROR);
    if (result == SOCKET_ERROR) {
        treatErrorExit("Server: bind error", -1, s);
    }
    DataPacket packet;
    std::cout << "Server: socket bound to address: " << address << " port: " << my_addr.sin_port << std::endl;
    
    bool serve = true;
    while (serve) {
        std::cout << "Server ready to recv" << std::endl;
        // receive the packet and write its data to the packet struct, from the address addr with a specific length
        int fromlen = sizeof(SOCKADDR);
        sockaddr_in client_addr;
        //address in recvfrom is an OUT PARAM!! that is it is the IP:port of the client!!! -> use another variable, as this gets confusing
        result = recvfrom(s, (char*)&packet, sizeof(DataPacket), 0, (SOCKADDR*)&client_addr,&fromlen);
        //assert(result != SOCKET_ERROR);
        if (result == SOCKET_ERROR) {
            treatErrorExit("Server: bind error", s, -1);
        }

        DataPacket clientPacket = (DataPacket)packet;

        //do something
        cout << "ClientPacket: " << clientPacket << endl;
        //send response to Client
        result = sendto(s, (char*)&clientPacket, sizeof(DataPacket), 0, (SOCKADDR*)&client_addr, fromlen);
        assert(result != SOCKET_ERROR);
        cout << "Server succesffully ecoed ClientPacket: " << clientPacket << endl;
        
        /*string strMsg = string(clientPacket.msg);
        if (!strMsg.compare(0,12,"Close_Server")) { //compare returns 0 when equal, we compare just the desired length
            serve = false; //will exit the loop and clean before returning
        }*/
    }
    std::cout << "Server: cleaning up and returning" << endl;
    // cleanup
    closesocket(s);
    WSACleanup();

}
