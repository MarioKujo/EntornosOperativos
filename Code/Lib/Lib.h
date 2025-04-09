#pragma once
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <ostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <assert.h>
#include "../Game/Game.hpp"

#define MSG_SIZE 256
enum Operation
{
    MOVE, INSPECT, DIG, USEMAP, PLACEFLAG, EAT, SONAR, EXIT
};
enum CellInfo
{
    NOTHING, TRAP, TREASURE, FLAG
};
typedef class DataPacket {
public:
    int client_id;
    enum Operation operation;
    int dx, dy; // Only for Moving
    bool isDug; // Only for inspecting
    enum CellInfo cellInfo; // Only for Digging
    bool trapNearby; // Only for map
    bool treasureNearby; // Only for map
    bool sonar; // Only for sonar
    char dir; // Only for sonar
    int energy;
    int currentTurn;
    int maxTurns;
    int treasuresFound;
    bool isRunning;
    Position position;
    DataPacket() :client_id(0), operation(MOVE), cellInfo(NOTHING),
        trapNearby(false), treasureNearby(false), sonar(false),
        dir(' '), energy(0), dx(0), dy(0),
        currentTurn(0), maxTurns(0), treasuresFound(0),
        isRunning(false), position{0, 0}, isDug(false) {};
    DataPacket(int _client_id, enum Operation _operation, enum CellInfo _cellDug,
        bool _trapNearby, bool _treasureNearby, bool _sonar,
        char _dir, int _energy, int _dx, int _dy, 
        int _currentTurn, int _maxTurns,int _treasuresFound, 
        bool _isRunning, Position _position, bool _isDug) {
        client_id = _client_id;
        operation = _operation;
        cellInfo = _cellDug;
        trapNearby = _trapNearby;
        treasureNearby = _treasureNearby;
        sonar = _sonar;
        dir = _dir;
        energy = _energy;
        position = _position;
        dx = _dx;
        dy = _dy;
        isDug = _isDug;
        currentTurn = _currentTurn;
        maxTurns = _maxTurns;
        treasuresFound = _treasuresFound;
        isRunning = _isRunning;
    }
} *PDataPacket;

typedef class ThreadInfo {
public:
    int thread_id;
    SOCKET s;
    std::string prefix;
    ThreadInfo() :thread_id(0), s(INVALID_SOCKET), prefix("") {};
    ThreadInfo(int _thread_id, SOCKET _s, std::string _prefix) {
        thread_id = _thread_id;
        s = _s;
        prefix = _prefix.c_str();
    }
    ~ThreadInfo() {
        closesocket(s);
    }
} *PThreadInfo;

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