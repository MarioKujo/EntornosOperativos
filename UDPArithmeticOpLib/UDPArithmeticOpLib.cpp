// UDPLib1.cpp : Define las funciones de la biblioteca estática.
//
#include <iostream>
#include <ostream>
#include "pch.h"
#include "framework.h"
#include "UDPArithmeticOpLib.h"


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


//TODO:performs sendto, assuming all required WinSock2 previous calls were succesfull
int sendtoMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, std::string prefix) {
    //now we just send the data through the socket
    int result = sendto(s, (char*)packet, sizeof(DataPacket), 0, (SOCKADDR*)dest_addr, sizeof(sockaddr_in));
    if (result < 0) {
        treatError(format("{}: error while sending: ", prefix), s);
        return SOCKET_ERROR;
    }

    std::cout << "Client:Succesfully sent msg" << std::endl;
    return result;
}
//TODO:performs recvfrom, assuming all required WinSock2 previous calls were succesfull
int recvfromMsg(SOCKET s, sockaddr_in* sender_addr, PDataPacket response, std::string prefix) {
    int fromlen = sizeof(sockaddr_in);
    //receive response
    int result = recvfrom(s, (char*)response, sizeof(DataPacket), 0, (SOCKADDR*)sender_addr, &fromlen);
    if (result == SOCKET_ERROR) {
        treatError(format("{}: error while receiving: ", prefix), s);
        return SOCKET_ERROR;
    }
    return result;
}

//TODO:performs sendtoMsg and then recvfromMsg, assuming all required WinSock2 previous calls were succesfull
int sendtorecvfromMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, PDataPacket response, std::string prefix) {
    int result = sendtoMsg(s, dest_addr, packet, prefix);
    result = recvfromMsg(s, dest_addr, response, prefix);
    return result;
}
//TODO:performs recvfromMsg and then sendtoMsg, assuming all required WinSock2 previous calls were succesfull
int recvfromsendtoMsg(SOCKET s, PDataPacket response, std::string prefix) {
    sockaddr_in dest_addr;
    int result = recvfromMsg(s, &dest_addr, response, prefix);
    result = sendtoMsg(s, &dest_addr, response, prefix);
    return result;
}