// UDPLib1.cpp : Define las funciones de la biblioteca estática.
//
#include <iostream>
#include <ostream>
#include "pch.h"
#include "framework.h"
#include "TCPArithmeticOpLibEmpty.h"
#include <format>



std::ostream& operator << (std::ostream& os, const DataPacket& dp) {
    return (os << "DataPacket{client: " << dp.client_id
        << " seq: " << dp.sequence
        << " op1: " << dp.op1
        << " operation: " << dp.operation
        << " op2: " << dp.op2
        << " res: " << dp.res << "}");
}

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
    return 0;
}

//performs recvfrom, assuming all required WinSock2 previous calls were succesfull
int recvfromMsg(SOCKET s, sockaddr_in* sender_addr, PDataPacket response, std::string prefix) {
    return 0;
}

//performs sendto and then recvfrom, assuming all required WinSock2 previous calls were succesfull
int sendtorecvfromMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, PDataPacket response, std::string prefix) {
    return 0;
}

//performs recvfrom and then sendto, assuming all required WinSock2 previous calls were succesfull
int recvfromsendtoMsg(SOCKET s, PDataPacket response, std::string prefix) {
    return 0;
}


//TCP calls

//TODO:performs send, assuming all required WinSock2 previous calls were succesfull
int sendMsg(SOCKET acceptSocket, PDataPacket packet, std::string prefix) {
    //IMPORTANT: size of struct/class not the size of the pointer!
    return 0;
}

//TODO:performs recv, assuming all required WinSock2 previous calls were succesfull
int recvMsg(SOCKET acceptSocket, PDataPacket recv_msg, std::string prefix) {
    return 0;
}

//TODO:performs send and then recv, assuming all required WinSock2 previous calls were succesfull
int sendrecvMsg(SOCKET s, PDataPacket packet, PDataPacket response, std::string prefix) {
    return 0;
}

//TODO:performs recv and then send, assuming all required WinSock2 previous calls were succesfull
int recvsendMsg(SOCKET s, PDataPacket response, std::string prefix) {
    return 0;
}