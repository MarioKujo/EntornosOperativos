
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>

#include <time.h>
#include <iostream>
#include <ostream>
#include <string>
#include <assert.h>

#include "../Game/Game.hpp"



#define WIDTH 10
#define HEIGHT 10
#define TREASURES 5
#define TURNS 30

enum Operation
{
    MOVE,
    INSPECT,
    DIG,
    USEMAP,
    PLACEFLAG,
    EAT,
    SONAR,
    EXIT
};

struct DataPacket
{
    int client_id = 0;
    Operation operation = Operation::MOVE;

    // Movement
    int dx = 0, dy = 0;

    // Inspect
    bool isDug = false;

    // Dig
    CellInfo cellInfo = CellInfo::NOTHING;

    // Map
    bool trapNearby = false;
    bool treasureNearby = false;

    // Sonar
    bool sonar = false;
    char dir = ' ';

    // Game status
    int energy = 0;
    int currentTurn = 0;
    int maxTurns = 0;
    int treasuresFound = 0;
    bool isRunning = false;
    bool canMove = false;
    Position position = { 0, 0 };

    DataPacket() = default;

    DataPacket(int clientId,
        Operation op,
        CellInfo cellInfo,
        bool trapNearby,
        bool treasureNearby,
        bool sonar,
        char dir,
        int energy,
        int dx,
        int dy,
        int currentTurn,
        int maxTurns,
        int treasuresFound,
        bool isRunning,
        bool canMove,
        const Position& pos,
        bool isDug)
        : client_id(clientId),
        operation(op),
        dx(dx),
        dy(dy),
        isDug(isDug),
        cellInfo(cellInfo),
        trapNearby(trapNearby),
        treasureNearby(treasureNearby),
        sonar(sonar),
        dir(dir),
        energy(energy),
        currentTurn(currentTurn),
        maxTurns(maxTurns),
        treasuresFound(treasuresFound),
        isRunning(isRunning),
        canMove(canMove),
        position(pos) {}
};

using PDataPacket = DataPacket*;

struct ThreadInfo
{
    int thread_id = 0;
    SOCKET s = INVALID_SOCKET;
    std::string prefix;

    ThreadInfo() = default;

    ThreadInfo(int threadId, SOCKET socket, const std::string& prefix)
        : thread_id(threadId), s(socket), prefix(prefix) {}

    ~ThreadInfo() {
        closesocket(s);
    }
};

using PThreadInfo = ThreadInfo*;

void treatError(const std::string msg, SOCKET s);

void treatErrorExit(const std::string msg, SOCKET s, int error);

//UDP calls

int sendtoMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, std::string prefix);

int recvfromMsg(SOCKET s, sockaddr_in* sender_addr, PDataPacket response, std::string prefix);

int sendtorecvfromMsg(SOCKET s, sockaddr_in* dest_addr, PDataPacket packet, PDataPacket response, std::string prefix);

int recvfromsendtoMsg(SOCKET s, PDataPacket response, std::string prefix);

//TCP calls

int sendMsg(SOCKET s, PDataPacket packet, std::string prefix);

int recvMsg(SOCKET s, PDataPacket response, std::string prefix);

int sendrecvMsg(SOCKET s, PDataPacket packet, PDataPacket response, std::string prefix);

int recvsendMsg(SOCKET s, PDataPacket response, std::string prefix);

int getAssignedPort(SOCKET s, sockaddr_in* my_addr);