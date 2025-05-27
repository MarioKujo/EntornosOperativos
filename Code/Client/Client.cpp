#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <assert.h>

#include "../Lib/Lib.h"
#include "../Game/Game.hpp"

using namespace std;

// Pauses and clears the console screen
void clearScreen()
{
    system("pause");
    system("CLS");
}

// Displays sonar warning messages based on nearby treasures or traps
void displaySonarWarning(bool treasureNearby, bool trapNearby)
{
    if (trapNearby && treasureNearby)
    {
        cout << "Be careful! There are traps and treasures nearby!" << endl;
    }
    else if (trapNearby)
    {
        cout << "Danger! Trap nearby!" << endl;
    }
    else if (treasureNearby)
    {
        cout << "Treasure nearby! Keep looking." << endl;
    }
    else if (!trapNearby && !treasureNearby)
    {
        cout << "No signal of treasures nor traps nearby." << endl;
    }
}

// Requests a new port from the server and updates the server address accordingly
int obtainNewPort(SOCKET s, sockaddr_in* serverAddr, const string& prefix)
{
    PDataPacket request = new DataPacket();  // Create an empty request packet
    PDataPacket response = new DataPacket(); // Create an empty response packet
    sendtorecvfromMsg(s, serverAddr, request, response, prefix); // Send request and receive response
    return 0;
}

// Initializes WinSock library for network communication
void initializeWinSock()
{
    WSAData wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    assert(result == NO_ERROR);
    cout << "Client: WinSock started correctly" << endl;
}

// Creates and returns a UDP socket for communication
SOCKET createSocket()
{
    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == INVALID_SOCKET)
    {
        treatErrorExit("Client: Socket creation error: ", s, -1);
    }
    return s;
}

// Configures server address struct with given IP and port
sockaddr_in configureServerAddress(const char* ip, int port)
{
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &(serverAddr.sin_addr.s_addr));
    return serverAddr;
}

// Closes the socket and cleans up WinSock resources
void shutdownClient(SOCKET s)
{
    if (closesocket(s) == SOCKET_ERROR)
    {
        cerr << "closesocket failed with error: " << WSAGetLastError() << endl;
    }
    WSACleanup();
    cout << "Client finishing..." << endl;
}

// Gets the player's action and related input (displacement or sonar direction)
int getPlayerAction(int& x, int& y, char& dir)
{
    int action;
    cin >> action;

    if (action == 1)
    {
        // If move action, get displacement values
        cout << "x displacement: "; cin >> x;
        cout << "y displacement: "; cin >> y;
    }
    else if (action == 7)
    {
        // If sonar action, get direction
        cout << "Choose a direction to fire the sonar (N/S/E/W): ";
        cin >> dir;
        dir = toupper(dir);
    }

    return action;
}

// Process server response for a movement action
void processMovementResponse(Player& player, PDataPacket response)
{
    if (response->canMove)
    {
        player.setPosition(response->position);
    }
    else
    {
        cout << "Invalid movement." << endl;
    }
    player.setEnergy(response->energy);
}

// Process response for a dig preview (before digging)
void processDigPreviewResponse(PDataPacket response)
{
    if (response->isDug) cout << "Cell has already been dug." << endl;
    if (response->cellInfo == FLAG) cout << "There's a flag here." << endl;
    if (!response->isDug) cout << "Cell hasn't been dug." << endl;
}

// Process dig action and update the game/map state
void processDigResponse(Game& game, Player& player, Map& map, Cell& cell, PDataPacket response, CellInfo& cellInfo)
{
    if (!response->isDug)
    {
        cout << "Cell has already been dug." << endl;
        return;
    }

    cellInfo = response->cellInfo;
    cell.isDug = true;
    cell.hasFlag = false;
    map.setCell(player.getPosition().x, player.getPosition().y, cell);
    player.setEnergy(response->energy);

    switch (cellInfo)
    {
    case NOTHING:   cout << "There's nothing here." << endl; break;
    case TREASURE:
        cout << "Treasure found!" << endl;
        game.setTreasuresFound(response->treasuresFound);
        break;
    case TRAP:
        cout << "It's a trap!" << endl;
        break;
    default:
        break;
    }
}

// Process flag action and update map if applicable
void processFlagResponse(Player& player, Map& map, Cell& cell, PDataPacket response)
{
    if (response->cellInfo == FLAG)
    {
        cell.hasFlag = true;
        map.setCell(player.getPosition().x, player.getPosition().y, cell);
        cout << "Cell (" << player.getPosition().x << ", " << player.getPosition().y << ") has been flagged." << endl;
    }
    else if (response->isDug)
    {
        cout << "Can't place a flag here, because the cell has already been dug." << endl;
    }
}

// Process energy replenishment
void processEnergyResponse(Player& player, PDataPacket response)
{
    player.setEnergy(response->energy);
    cout << "You replenished your energy. Current energy: " << player.getEnergy() << endl;
}

// Process sonar direction result
void processSonarDirectionResponse(bool sonar)
{
    cout << (sonar ? "The sonar detects something in that direction." :
        "Nothing detected in that direction.") << endl;
}

// Handles the response from the server based on the player's action
void handleServerResponse(Game& game, Player& player, Map& map, Cell& cell, PDataPacket response, int action, CellInfo& cellInfo, bool& treasureNearby, bool& trapNearby, bool& sonar, bool& isRunning)
{
    // Update shared status flags from the server response
    treasureNearby = response->treasureNearby;
    trapNearby = response->trapNearby;
    sonar = response->sonar;
    isRunning = response->isRunning;

    switch (action)
    {
        case 1: 
        {
            processMovementResponse(player, response);
            break;
        }
        case 2: 
        {
            processDigPreviewResponse(response);
            break;
        }
        case 3: 
        {
            processDigResponse(game, player, map, cell, response, cellInfo);
            break;
        }
        case 4:
        {
            displaySonarWarning(treasureNearby, trapNearby);
            break;
        }
        case 5:
        {
            processFlagResponse(player, map, cell, response);
            break;
        }
        case 6:
        {
            processEnergyResponse(player, response);
            break;
        }
        case 7:
        {
            processSonarDirectionResponse(sonar);
            break;
        }
    }

    clearScreen();
}

// Main game loop handling input, communication, and state updates
void runGameLoop(SOCKET s, sockaddr_in& serverAddr, const string& prefix, Game& game, int clientId)
{
    bool isRunning = true;
    bool treasureNearby = false, trapNearby = false, sonar = false;
    int treasuresFound = 0;
    CellInfo cellInfo = NOTHING;
    Player& player = game.getPlayer();

    while (isRunning)
    {
        game.run();

        int x = 0, y = 0;
        char dir = ' ';
        int action = getPlayerAction(x, y, dir);

        // Prepare request packet to send to server
        PDataPacket request = new DataPacket(clientId, static_cast<Operation>(action - 1), cellInfo,
            treasureNearby, trapNearby, sonar, dir,
            player.getEnergy(), x, y, game.getCurrentTurn(),
            game.getTurnLimit(), treasuresFound,
            game.getIsRunning(), false, player.getPosition(),
            false);

        PDataPacket response = new DataPacket();

        // Send request and receive server response
        sendtorecvfromMsg(s, &serverAddr, request, response, prefix);

        game.setCurrentTurn(response->currentTurn);

        Map& map = game.getMap();
        Cell& cell = map.getCell(player.getPosition().x, player.getPosition().y);

        // Handle server response
        handleServerResponse(game, player, map, cell, response, action, cellInfo, treasureNearby, trapNearby, sonar, isRunning);
    }
}

int main(int argc, char* argv[])
{
    // Check for client ID argument
    if (argc < 2)
    {
        cout << "Usage: ./UDPClient <id>" << endl;
        return -1;
    }

    int clientId = atoi(argv[1]);
    cout << "Client: starting..." << endl;

    // Initialize networking
    initializeWinSock();
    SOCKET s = createSocket();

    // Set server address to localhost and port 4000
    sockaddr_in serverAddr = configureServerAddress("127.0.0.1", 4000);

    string prefix = "Client:";

    // Obtain new port from server
    obtainNewPort(s, &serverAddr, prefix);
    cout << "New port obtained: " << ntohs(serverAddr.sin_port) << endl;

    // Initialize game instance
    Game game(WIDTH, HEIGHT, TURNS);
    system("CLS");

    // Run main game loop
    runGameLoop(s, serverAddr, prefix, game, clientId);

    // Cleanup and close
    shutdownClient(s);

    return 0;
}
