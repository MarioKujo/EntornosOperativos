
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
#include <nlohmann/json.hpp>

#include "../Game/Game.hpp"

#define WIDTH 10
#define HEIGHT 10
#define TREASURES 5
#define TURNS 30

using json = nlohmann::json;

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

// Structure representing the packet of data exchanged between client and server
struct DataPacket
{
    int client_id = 0;                         // ID of the client sending/receiving the packet
    Operation operation = Operation::MOVE;     // Current operation the client is performing

    // Movement data
    int dx = 0, dy = 0;                        // Movement direction (delta x, delta y)

    // Inspect operation
    bool isDug = false;                        // Whether a cell has been dug

    // Dig operation
    CellInfo cellInfo = CellInfo::NOTHING;     // Information about the cell after digging

    // Map operation
    bool trapNearby = false;                   // Whether a trap is detected nearby
    bool treasureNearby = false;               // Whether a treasure is detected nearby

    // Sonar operation
    bool sonar = false;                        // Whether sonar was used
    char dir = ' ';                            // Direction used by sonar (N, S, E, W, etc.)

    // Game status
    int energy = 0;                            // Current energy level of the client
    int currentTurn = 0;                       // Current turn number
    int maxTurns = 0;                          // Maximum allowed turns in the game
    int treasuresFound = 0;                    // Number of treasures found
    bool isRunning = false;                    // Whether the game is currently running
    bool canMove = false;                      // Whether the player can move
    Position position = { 0, 0 };              // Current position of the client

    // Default constructor
    DataPacket() = default;

    // Custom constructor for quickly initializing all fields
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

// JSON serialization: convert DataPacket --> json
inline void to_json(json& j, const DataPacket& p)
{
    j = json
    {
        {"client_id", p.client_id},
        {"operation", p.operation},
        {"dx", p.dx}, {"dy", p.dy},
        {"isDug", p.isDug},
        {"cellInfo", p.cellInfo},
        {"trapNearby", p.trapNearby},
        {"treasureNearby", p.treasureNearby},
        {"sonar", p.sonar},
        {"dir", p.dir},
        {"energy", p.energy},
        {"currentTurn", p.currentTurn},
        {"maxTurns", p.maxTurns},
        {"treasuresFound", p.treasuresFound},
        {"isRunning", p.isRunning},
        {"canMove", p.canMove},
        {"position", {{"x", p.position.x}, {"y", p.position.y}}}
    };
}

// JSON deserialization: convert json --> DataPacket
inline void from_json(const json& j, DataPacket& p)
{
    j.at("client_id").get_to(p.client_id);
    j.at("operation").get_to(p.operation);
    j.at("dx").get_to(p.dx);
    j.at("dy").get_to(p.dy);
    j.at("isDug").get_to(p.isDug);
    j.at("cellInfo").get_to(p.cellInfo);
    j.at("trapNearby").get_to(p.trapNearby);
    j.at("treasureNearby").get_to(p.treasureNearby);
    j.at("sonar").get_to(p.sonar);
    j.at("dir").get_to(p.dir);
    j.at("energy").get_to(p.energy);
    j.at("currentTurn").get_to(p.currentTurn);
    j.at("maxTurns").get_to(p.maxTurns);
    j.at("treasuresFound").get_to(p.treasuresFound);
    j.at("isRunning").get_to(p.isRunning);
    j.at("canMove").get_to(p.canMove);
    j.at("position").at("x").get_to(p.position.x);
    j.at("position").at("y").get_to(p.position.y);
}

// JSON serialization for Position
inline void to_json(json& j, const Position& p)
{
    j = json{ {"x", p.x}, {"y", p.y} };
}

// JSON deserialization for Position
inline void from_json(const json& j, Position& p)
{
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
}

// Enum serialization for Operation using nlohmann::json macro
NLOHMANN_JSON_SERIALIZE_ENUM(Operation,
    {
        {MOVE, "MOVE"},
        {INSPECT, "INSPECT"},
        {DIG, "DIG"},
        {USEMAP, "USEMAP"},
        {PLACEFLAG, "PLACEFLAG"},
        {EAT, "EAT"},
        {SONAR, "SONAR"},
        {EXIT, "EXIT"}
    });

// Enum serialization for CellInfo using nlohmann::json macro
NLOHMANN_JSON_SERIALIZE_ENUM(CellInfo,
    {
        {NOTHING, "NOTHING"},
        {TRAP, "TRAP"},
        {TREASURE, "TREASURE"},
        {FLAG, "FLAG"}
    });

// Alias for pointer to DataPacket
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