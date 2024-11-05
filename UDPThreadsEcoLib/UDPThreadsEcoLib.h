#pragma once
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <ostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <assert.h>
#define MSG_SIZE 256

typedef class DataPacket {
public:
    int client_id;
    int sequence;
    char msg[MSG_SIZE];
    DataPacket() :client_id(0), sequence(0), msg{ NULL } {}
    DataPacket(int _client_id, int _sequence, std::string _msg) {
        client_id = _client_id;
        sequence = _sequence;
        //deep copy of string into the struct so that data reaches the server, NEVER send a pointer / shallow copy
        memset(msg, 0, MSG_SIZE); //clear all the memory in packet.msg before copying a new msg
        _msg.copy(msg, _msg.size(), 0);
    }
} *PDataPacket;


std::ostream& operator << (std::ostream& os, const DataPacket& dp);
// The reason this is in a separate file is because I want to use this
// on the server and the client

void treatError(const std::string msg, SOCKET s);

void treatErrorExit(const std::string msg, SOCKET s, int error);

int sendtoMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, std::string prefix);

int recvfromMsg(SOCKET s, sockaddr_in* sender_addr, PDataPacket response, std::string prefix);

int sendtorecvfromMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, PDataPacket response, std::string prefix);

int recvfromsendtoMsg(SOCKET s, PDataPacket response, std::string prefix);
