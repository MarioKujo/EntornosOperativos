#pragma once
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <ostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <assert.h>
#include <format>
#define MSG_SIZE 256
enum Operation
{
    SUM, DIFF, PROD, DIV, POWER
};



typedef class DataPacket {
public:
    int client_id;
    int sequence;
    int op1;
    enum Operation operation;
    //int operation;
    int op2;
    long long res;
    DataPacket() {};
    DataPacket(int _client_id, int _sequence, int _op1, enum Operation _operation, int _op2) {
        //DataPacket(int _client_id, int _sequence, int _op1, int _operation, int _op2) {
        client_id = _client_id;
        sequence = _sequence;
        op1 = _op1;
        operation = _operation;
        op2 = _op2;
        res = INFINITE;
    }
} *PDataPacket;


std::ostream& operator << (std::ostream& os, const DataPacket& dp);
// The reason this is in a separate file is because I want to use this
// on the server and the client

void treatError(const std::string msg, SOCKET s);

void treatErrorExit(const std::string msg, SOCKET s, int error);

int udpCommonSocketSetup(SOCKET s, PCSTR address, u_short port, sockaddr_in* addr);

int udpServerSocketSetup(SOCKET s, PCSTR address, u_short port, sockaddr_in* addr);

int sendtoMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, std::string prefix);

int recvfromMsg(SOCKET s, sockaddr_in* sender_addr, PDataPacket response, std::string prefix);

int sendtorecvfromMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, PDataPacket response, std::string prefix);

int recvfromsendtoMsg(SOCKET s, PDataPacket response, std::string prefix);
