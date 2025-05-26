// UDPLib1.cpp : Define las funciones de la biblioteca estática.
//
#include <iostream>
#include <ostream>
#include "pch.h"
#include "framework.h"
#include "Lib.h"
#include <format>

//will print msg with WSAGetLastError, then closesocket
void treatError(const std::string msg, SOCKET s)
{
    std::cout << msg << WSAGetLastError() << std::endl;
    closesocket(s);
}

//will print msg with WSAGetLastError, then closesocket, WSACleanup and ExitProcess(error)
void treatErrorExit(const std::string msg, SOCKET s, int error)
{
    std::cout << msg << WSAGetLastError() << std::endl;
    closesocket(s);
    WSACleanup();
    ExitProcess(error);
}

//UDP calls

int sendtoMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, std::string prefix)
{
    // Serialize the DataPacket object to JSON
    json j = *packet;

    // Convert the JSON object to a string
    std::string msg = j.dump();

    // Send the serialized JSON string as a UDP datagram
    int result = sendto(s, msg.c_str(), (int)msg.size(), 0, (SOCKADDR*)dest_addr, sizeof(SOCKADDR));

    // Ensure the send was successful
    assert(result != SOCKET_ERROR);

    return result;
}

int recvfromMsg(SOCKET s, sockaddr_in* sender_addr, PDataPacket response, std::string prefix)
{
    // Create a buffer to store the incoming data
    char buffer[4096];

    // Variable to hold the size of the sender's address structure
    int fromlen = sizeof(SOCKADDR);

    // Receive data from the socket into the buffer
    int result = recvfrom(s, buffer, sizeof(buffer) - 1, 0, (SOCKADDR*)sender_addr, &fromlen);

    // Ensure the receive was successful
    assert(result != SOCKET_ERROR);

    // Null-terminate the received data to safely convert it into a string
    buffer[result] = '\0';

    // Parse the received JSON string into a JSON object
    json j = json::parse(buffer);

    // Deserialize the JSON object into a DataPacket
    *response = j.get<DataPacket>();

    return result;
}


//performs sendto and then recvfrom, assuming all required WinSock2 previous calls were succesfull
int sendtorecvfromMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, PDataPacket response, std::string prefix)
{
    sendtoMsg(s, dest_addr, packet, prefix);
    return recvfromMsg(s, dest_addr, response, prefix);
}

//performs recvfrom and then sendto, assuming all required WinSock2 previous calls were succesfull
int recvfromsendtoMsg(SOCKET s, PDataPacket response, std::string prefix)
{
    sockaddr_in sender_addr;
    recvfromMsg(s, &sender_addr, response, prefix);
    return sendtoMsg(s, &sender_addr, response, prefix);
}