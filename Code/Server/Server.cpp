#pragma comment(lib, "ws2_32.lib")

#include "../Lib/Lib.h"

#pragma comment(lib, "ws2_32.lib")

#include "../Lib/Lib.h"
#include "../Game/Game.hpp"

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <format>


#include <stdio.h>
#include <math.h>
#include <format>

using namespace std;

#define MAX_THREADS 5

HANDLE serverFun(PDataPacket clientPacket, SOCKET s, sockaddr_in* client_addr, int i, string prefix);
int serverThreadFun(PDataPacket clientPacket, Game* game);
DWORD WINAPI threadFun(LPVOID param);

// Initialize WinSock library for network communication
int initializeWinSock()
{
    WSAData wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData); // Request WinSock version 2.2
    if (result != NO_ERROR)
    {
        cerr << "Server: Failed to start WinSock. Error: " << result << endl;
        return result; // Return error code if initialization failed
    }
    cout << "Server: WinSock started correctly" << endl;
    return 0; // Success
}

// Create a UDP socket, bind it to the specified IP and port
SOCKET createAndBindSocket(const char* ip, int port)
{
    // Create socket (IPv4, UDP)
    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (s == INVALID_SOCKET)
    {
        cerr << "Server: Socket creation error." << endl;
        return INVALID_SOCKET; // Return error if socket creation fails
    }

    cout << "Server: socket created" << endl;

    // Setup socket address structure
    sockaddr_in my_addr{};
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port); // Convert port to network byte order

    if (!inet_pton(AF_INET, ip, &(my_addr.sin_addr.s_addr)))
    {
        cerr << "Server: Error converting IP string to binary." << endl;
        closesocket(s); // Clean up socket before returning error
        return INVALID_SOCKET;
    }

    // Bind socket to the specified IP and port
    if (::bind(s, (sockaddr*)&my_addr, sizeof(my_addr)) == SOCKET_ERROR)
    {
        cerr << "Server: Bind error." << endl;
        closesocket(s); // Clean up socket before returning error
        return INVALID_SOCKET;
    }

    cout << "Server: socket bound to address: " << ip << " port: " << port << endl;
    return s; // Return the created and bound socket
}

int main()
{
    constexpr int port = 4000;
    constexpr const char* ip = "127.0.0.1";
    const string prefix = "Server: ";

    cout << prefix << "starting..." << endl;

    // Initialize WinSock
    if (initializeWinSock() != 0)
    {
        return -1; // Exit if WinSock initialization failed
    }

    // Create and bind UDP socket to specified IP and port
    SOCKET s = createAndBindSocket(ip, port);
    if (s == INVALID_SOCKET)
    {
        WSACleanup(); // Clean up WinSock before exiting
        return -1;
    }

    // Allocate memory for receiving data packets
    PDataPacket packet = new DataPacket();

    HANDLE hThreadArray[MAX_THREADS];
    int threadCount = 0;

    // Main loop: receive packets and spawn threads to handle clients
    while (threadCount < MAX_THREADS)
    {
        cout << prefix << "ready to recv" << endl;

        sockaddr_in client_addr{}; // Structure to store client address
        recvfromMsg(s, &client_addr, packet, prefix); // Receive data from client

        // Create a new thread to handle the client request
        hThreadArray[threadCount] = serverFun(packet, s, &client_addr, threadCount, prefix);
        threadCount++;
    }

    // Wait for all client threads to finish execution
    WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

    // Close all thread handles
    for (int i = 0; i < MAX_THREADS; i++)
    {
        CloseHandle(hThreadArray[i]);
    }

    // Free allocated memory and cleanup resources
    delete packet;
    closesocket(s);
    WSACleanup();

    cout << prefix << "cleaning up and exiting." << endl;
    return 0;
}


// Creates a UDP socket bound to the given IP and port (0 means OS assigns port)
// Returns INVALID_SOCKET on failure
SOCKET createBoundSocket(int port = 0, const char* ip = "127.0.0.1")
{
    SOCKET s_new = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (s_new == INVALID_SOCKET)
    {
        cerr << "Server: Failed to create socket." << endl;
        return INVALID_SOCKET;
    }

    sockaddr_in my_addr{};
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);

    if (!inet_pton(AF_INET, ip, &(my_addr.sin_addr.s_addr)))
    {
        cerr << "Server: Failed to convert IP string to binary." << endl;
        closesocket(s_new);
        return INVALID_SOCKET;
    }

    if (::bind(s_new, (sockaddr*)&my_addr, sizeof(my_addr)) == SOCKET_ERROR)
    {
        cerr << "Server: Failed to bind socket." << endl;
        closesocket(s_new);
        return INVALID_SOCKET;
    }

    // Optional: print assigned port if port was 0
    sockaddr_in bound_addr{};
    int addr_len = sizeof(bound_addr);

    if (getsockname(s_new, (sockaddr*)&bound_addr, &addr_len) == 0)
    {
        cout << "Server: socket bound to address: " << ip << " port: " << ntohs(bound_addr.sin_port) << endl;
    }

    return s_new;
}

// Creates a thread to handle client communication, returns thread HANDLE or NULL on failure
HANDLE createClientThread(int i, SOCKET s_new, const string& prefix)
{
    PThreadInfo thInfo = new ThreadInfo(i, s_new, prefix);

    DWORD dwThreadId;
    HANDLE hThread = CreateThread(
        NULL,           // default security attributes
        0,              // default stack size
        threadFun,      // thread function
        (void*)thInfo,  // argument to thread function
        0,              // default creation flags
        &dwThreadId     // returns thread ID
    );

    if (hThread == NULL)
    {
        cerr << prefix << "Failed to create thread for client " << i << endl;
        delete thInfo; // Avoid memory leak
    }

    return hThread;
}

// Main server function to create socket, send initial packet and create client thread
HANDLE serverFun(PDataPacket clientPacket, SOCKET s, sockaddr_in* client_addr, int i, string prefix)
{
    SOCKET s_new = createBoundSocket(0, "127.0.0.1");

    if (s_new == INVALID_SOCKET)
    {
        treatErrorExit(format("Server: Thread[{}] socket creation or bind error.", i), s, -1);
    }

    sendtoMsg(s_new, client_addr, clientPacket, prefix);

    HANDLE hThread = createClientThread(i, s_new, prefix);

    if (hThread == NULL)
    {
        treatErrorExit("Server: CreateThread failed", s, -1);
        ExitProcess(3);
    }

    return hThread;
}

// Thread function that serves a client with its own game instance
DWORD WINAPI threadFun(LPVOID param)
{
    PThreadInfo thInfo = (ThreadInfo*)param;
    bool serve = true;
    PDataPacket packet = new DataPacket();

    // Create a new game instance per thread/client
    Game g(WIDTH, HEIGHT, TREASURES, TURNS);

    while (serve)
    {
        cout << "Server Thread ready to recv" << endl;

        sockaddr_in client_addr{};
        recvfromMsg(thInfo->s, &client_addr, packet, "Server:");

        DataPacket clientPacket = (DataPacket)*packet;

        // Process the client packet
        if (!serverThreadFun(&clientPacket, &g))
        {
            sendtoMsg(thInfo->s, &client_addr, &clientPacket, thInfo->prefix);
        }

        else
        {
            // Exit thread if error or client requests exit
            serve = false;
        }
    }

    // Cleanup
    // closesocket(thInfo->s); // Not needed, done in ThreadInfo destructor
    delete thInfo;
    return 0;
}

// Handler functions for each operation
void handleMove(PDataPacket clientPacket, Player& player, Map& map)
{
    if (player.move(clientPacket->dx, clientPacket->dy, map))
    {
        clientPacket->energy = player.getEnergy();
        clientPacket->position = player.getPosition();
        clientPacket->canMove = true;
    }

    else
    {
        clientPacket->canMove = false;
    }
}

void handleInspect(PDataPacket clientPacket, Player& player, Map& map)
{
    InspectInfo iInfo = player.inspect(map);
    clientPacket->isDug = iInfo.isDug;
    clientPacket->cellInfo = (iInfo.hasFlag ? FLAG : NOTHING);
}

void handleDig(PDataPacket clientPacket, Player& player, Map& map)
{
    if (!map.getCell(player.getPosition().x, player.getPosition().y).isDug)
    {
        player.dig(map);
        map.getCell(player.getPosition().x, player.getPosition().y).hasFlag = false;

        clientPacket->isDug = true;
        clientPacket->energy = player.getEnergy();

        if (map.getCell(player.getPosition().x, player.getPosition().y).hasTreasure)
        {
            clientPacket->cellInfo = TREASURE;
            clientPacket->treasuresFound++;
        }

        else if (map.getCell(player.getPosition().x, player.getPosition().y).hasTrap)
        {
            clientPacket->cellInfo = TRAP;
        }

        else
        {
            clientPacket->cellInfo = NOTHING;
        }
    }
}

void handleUseMap(PDataPacket clientPacket, Player& player, Map& map)
{
    NearbyInfo nInfo = player.useMap(map);
    clientPacket->treasureNearby = nInfo.treasureNearby;
    clientPacket->trapNearby = nInfo.trapNearby;
}

void handlePlaceFlag(PDataPacket clientPacket, Player& player, Map& map)
{
    if (!map.getCell(player.getPosition().x, player.getPosition().y).isDug)
    {
        player.placeFlag(map);
        clientPacket->cellInfo = FLAG;
    }

    else
    {
        clientPacket->isDug = true;
    }
}

void handleEat(PDataPacket clientPacket, Player& player)
{
    player.eat();
    clientPacket->energy = player.getEnergy();
}

void handleSonar(PDataPacket clientPacket, Player& player, Map& map)
{
    clientPacket->sonar = player.useSonar(map, clientPacket->dir, player.getPosition().x, player.getPosition().y);
}

// Main refactored server thread function
int serverThreadFun(PDataPacket clientPacket, Game* game)
{
    cout << "Server received packet" << endl;

    Player& player = game->getPlayer();
    Map& map = game->getMap();

    // Sync player state from client packet
    player.setEnergy(clientPacket->energy);
    player.setPosition(clientPacket->position);

    switch (clientPacket->operation)
    {
        case MOVE:
        {
            handleMove(clientPacket, player, map);
            break;
        }

        case INSPECT:
        {
            handleInspect(clientPacket, player, map);
            break;
        }

        case DIG:
        {
            handleDig(clientPacket, player, map);
            break;
        }

        case USEMAP:
        {
            handleUseMap(clientPacket, player, map);
            break;
        }

        case PLACEFLAG:
        {
            handlePlaceFlag(clientPacket, player, map);
            break;
        }

        case EAT:
        {
            handleEat(clientPacket, player);
            break;
        }

        case SONAR:
        {
            handleSonar(clientPacket, player, map);
            break;
        }

        case EXIT:
        {
            clientPacket->isRunning = false;
            break;
        }
    }

    // Advance turn number unless it's the last turn
    if (clientPacket->currentTurn != clientPacket->maxTurns)
    {
        clientPacket->currentTurn++;
    }
    else
    {
        clientPacket->isRunning = false;
    }

    // End game if player runs out of energy
    if (clientPacket->energy <= 0)
    {
        clientPacket->isRunning = false;
    }

    return 0;
}
