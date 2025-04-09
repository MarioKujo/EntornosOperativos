// UDPLib1.cpp : Define las funciones de la biblioteca estática.
//
#include <iostream>
#include <ostream>
#include "pch.h"
#include "framework.h"
#include "Lib.h"
#include <format>

//will print msg with WSAGetLastError, then closesocket
void treatError(const std::string msg, SOCKET s) {
    std::cout << msg << WSAGetLastError() << std::endl;
    closesocket(s);
}

//will print msg with WSAGetLastError, then closesocket, WSACleanup and ExitProcess(error)
void treatErrorExit(const std::string msg, SOCKET s, int error) {
    std::cout << msg << WSAGetLastError() << std::endl;
    closesocket(s);
    WSACleanup();
    ExitProcess(error);
}

//UDP calls

//performs sendto, assuming all required WinSock2 previous calls were succesfull
int sendtoMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, std::string prefix) {
    //now we just send the data through the socket
    //we make the casting to char* because it expects data as just chars, last param 0 is for flags that we don't need
    int result = sendto(s, (char*)packet, sizeof(DataPacket), 0, (SOCKADDR*)dest_addr, sizeof(SOCKADDR));
    assert(result != SOCKET_ERROR);
    return result;
}

//performs recvfrom, assuming all required WinSock2 previous calls were succesfull
int recvfromMsg(SOCKET s, sockaddr_in* sender_addr, PDataPacket response, std::string prefix) {
    //receive response from Server
    //DataPacket response;
    int fromlen = sizeof(SOCKADDR);
    //recvfrom addr is ALWAYS an out param 
    int result = recvfrom(s, (char*)response, sizeof(DataPacket), 0, (SOCKADDR*)sender_addr, &fromlen);
    assert(result != SOCKET_ERROR);
    return result;
}

//performs sendto and then recvfrom, assuming all required WinSock2 previous calls were succesfull
int sendtorecvfromMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, PDataPacket response, std::string prefix) {
    sendtoMsg(s, dest_addr, packet, prefix);
    return recvfromMsg(s, dest_addr, response, prefix);
}

//performs recvfrom and then sendto, assuming all required WinSock2 previous calls were succesfull
int recvfromsendtoMsg(SOCKET s, PDataPacket response, std::string prefix) {
    sockaddr_in sender_addr;
    recvfromMsg(s, &sender_addr, response, prefix);
    return sendtoMsg(s, &sender_addr, response, prefix);
}


//TCP calls

//performs send, assuming all required WinSock2 previous calls were succesfull
int sendMsg(SOCKET acceptSocket, PDataPacket packet, std::string prefix) {
    //IMPORTANT: size of struct/class not the size of the pointer!
    int sbyteCount = send(acceptSocket, (char*)packet, sizeof(DataPacket), 0);
    if (sbyteCount < 0) {
        treatError(std::format("{} send error: ", prefix), acceptSocket);
    }
    return sbyteCount;
}

//performs recv, assuming all required WinSock2 previous calls were succesfull
int recvMsg(SOCKET acceptSocket, PDataPacket recv_msg, std::string prefix) {
    int rbyteCount = recv(acceptSocket, (char*)recv_msg, sizeof(DataPacket), 0);
    if (rbyteCount < 0) {
        treatError(std::format("{} recv error: ", prefix), acceptSocket); //change when loop to treatError
    }
    else {
        DataPacket clientPacket = (DataPacket)*recv_msg;
    }
    return rbyteCount;
}

//performs send and then recv, assuming all required WinSock2 previous calls were succesfull
int sendrecvMsg(SOCKET s, PDataPacket packet, PDataPacket response, std::string prefix) {
    sendMsg(s, packet, prefix);
    return recvMsg(s, response, prefix);
}

//performs recv and then send, assuming all required WinSock2 previous calls were succesfull
int recvsendMsg(SOCKET s, PDataPacket response, std::string prefix) {
    recvMsg(s, response, prefix);
    return sendMsg(s, response, prefix);
}

//invokes getsockname and retrieves the port number in this machine's architecture encoding
int getAssignedPort(SOCKET s, sockaddr_in* my_addr) {
    int namelen = sizeof(my_addr);
    if (getsockname(s, (SOCKADDR*)my_addr, &namelen) != 0) {
        treatErrorExit("Server: getsockname error: ", s, -1);
    }
    int assigned_port = ntohs(my_addr->sin_port);
    std::cout << "Server: socket bound to port: " << my_addr->sin_port << std::endl;
    return assigned_port;
}