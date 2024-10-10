#pragma once
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <ostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

#define MSG_SIZE 256

typedef struct {
    int client_id;
    int sequence;
    char msg[MSG_SIZE];
} DataPacket;


std::ostream& operator << (std::ostream& os, const DataPacket& dp);
// The reason this is in a separate file is because I want to use this
// on the server and the client

void treatError(const std::string msg, SOCKET s);

void treatErrorExit(const std::string msg, SOCKET s, int error);