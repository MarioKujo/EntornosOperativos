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
    else
    {
        cout << "No signal of treasures nor traps nearby." << endl;
    }
}

// Requests a new port from the server and updates the server address accordingly
int obtainNewPort(SOCKET s, sockaddr_in* serverAddr, const string& prefix)
{
    PDataPacket request = new DataPacket();  // Create an empty request packet
    PDataPacket response = new DataPacket(); // Packet to receive server's response
    sendtorecvfromMsg(s, serverAddr, request, response, prefix);
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
        cout << "x displacement: "; cin >> x;
        cout << "y displacement: "; cin >> y;
    }
    else if (action == 7)
    {
        cout << "Choose a direction to fire the sonar (N/S/E/W): ";
        cin >> dir;
        dir = toupper(dir);
    }

    return action;
}

// Handles the response from the server based on the player's action
void handleServerResponse(Game& game, Player& player, Map& map, Cell& cell, PDataPacket response, int action, CellInfo& cellInfo, bool& treasureNearby, bool& trapNearby, bool& sonar, bool& isRunning)
{

    switch (action)
    {
	    case 1:
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
		    game.setPlayer(player);
		    break;
	    }
        case 2:
        {
            // Check if the cell is dug or contains a flag
            if (response->isDug) cout << "Cell has already been dug." << endl;
            if (response->cellInfo == FLAG) cout << "There's a flag here." << endl;
            if (!response->isDug) cout << "Cell hasn't been dug." << endl;
            break;
        }

        case 3:
        {
            if(response->isDug)
            {
                // Update cell info and map with the server response
                cellInfo = response->cellInfo;
                cell.isDug = response->isDug;
                cell.hasFlag = false;

                map.setCell(player.getPosition().x, player.getPosition().y, cell);
                game.setMap(map);

                player.setEnergy(response->energy);
                game.setPlayer(player);

                // Provide feedback depending on cell content
                switch (cellInfo)
                {
                case NOTHING:
                {
                    cout << "There's nothing here." << endl;
                    break;
                }
                case TREASURE:
                {
                    cout << "Treasure found!" << endl;
                    game.setTreasuresFound(response->treasuresFound);
                    break;
                }
                case TRAP:
                {
                    cout << "It's a trap!" << endl;
                    break;
                }
                default:
                    break;
                }
            }

            else
            {
                cout << "Cell has already been dug." << endl;
            }

            break;
        }

        case 4:
        {
            // Show sonar warnings about nearby traps or treasures
            displaySonarWarning(treasureNearby, trapNearby);
            break;
        }

        case 5:
        {
            // Mark the cell if a flag is detected
            if (response->cellInfo == FLAG)
            {
                cell.hasFlag = true;
                map.setCell(player.getPosition().x, player.getPosition().y, cell);
                game.setMap(map);
                cout << "Cell (" << player.getPosition().x << ", " << player.getPosition().y << ") has been flagged." << endl;
            }
            else if (response->isDug)
            {
                cout << "Can't place a flag here, because the cell has already been dug." << endl;
            }
            break;
        }

        case 6:
        {
            // Notify player of energy replenishment
            cout << "You replenished your energy. Current energy: " << player.getEnergy() << endl;
            break;
        }

        case 7:
        {
            // Output sonar detection result
            cout << (sonar ? "The sonar detects something in that direction." :
                "Nothing detected in that direction.") << endl;
            break;
        }

        default:
            break;
    }

    clearScreen();

    // Update flags based on server response
    treasureNearby = response->treasureNearby;
    trapNearby = response->trapNearby;
    sonar = response->sonar;
    isRunning = response->isRunning;
}

// Main game loop handling input, communication, and state updates
void runGameLoop(SOCKET s, sockaddr_in& serverAddr, const string& prefix, Game& game, int clientId)
{
    bool isRunning = true;
    bool treasureNearby = false, trapNearby = false, sonar = false;
    int treasuresFound = 0;
    CellInfo cellInfo = NOTHING;
    Player player = game.getPlayer();

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
        sendtorecvfromMsg(s, &serverAddr, request, response, prefix);

        game.setCurrentTurn(response->currentTurn);

        Map map = game.getMap();

        Cell cell = map.getCell(player.getPosition().x, player.getPosition().y);

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
