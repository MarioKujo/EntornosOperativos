#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <assert.h>
#include "../Lib/Lib.h"
#include "../Game/Game.hpp"
#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

int obtainNewPort(SOCKET s, sockaddr_in* server_addr, string prefix);

void clearScreen()
{
    system("pause");
    system("CLS");
}
int main(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "UDPClient usage: .\\UDPClient <id>" << endl;
        ExitProcess(-1);
    }
    int client = atoi(argv[1]);
    std::cout << "Client: starting..." << std::endl;
    //required intialization of WinSock 2 library, it writes some data om wsaData to check everything is ok
    int result;
    WSAData wsaData;
    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    assert(result == NO_ERROR);
    std::cout << "Client: WinSock started correctly" << std::endl;

    //now we create a socket that uses IP (AF_INET) with UDP (SOCK_DGRAM and IPPROTO_UDP) 
    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == INVALID_SOCKET) {
        treatErrorExit("Client: Socket creation error: ", s, -1);
    }
    std::cout << "Client: socket created" << std::endl;
    //now we specify the other machine we want to send messages to
    sockaddr_in server_addr;
    PCSTR address = "127.0.0.1";
    if (!inet_pton(AF_INET, address, &(server_addr.sin_addr.s_addr))) { // Replace with your desired IP address
        printf("error converting IP in string to binary\n");
        return -1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(4000);

    string prefix = "Client:";
    obtainNewPort(s, &server_addr, prefix);
    std::cout << "Client already obtained new port: " << ntohs(server_addr.sin_port) << std::endl;
    enum CellInfo cI = NOTHING; // To use cellInfo later
    // Creates a game with the map's width, height and the max number of turns (has to be the same as server's)
    Game game(WIDTH, HEIGHT, TURNS); 
    system("CLS");
    bool isRunning = true;
    int treasuresFound = 0;
    Player player = game.getPlayer();
    bool treasureNearby = false, trapNearby = false, sonar = false;
    while (isRunning)
    {
        game.run(); // Shows board and menu
        int action;
        cin >> action; // Inputs action
        int x = 0, y = 0;
        if (action == 1) // If action is move, inputs x and y displacement
        {
            cout << "x displacement: ";
            cin >> x;
            cout << "y displacement: ";
            cin >> y;
        }
        char dir = ' ';
        if (action == 7) // If action is sonar, chooses direction
        {
            cout << "Choose a direction to fire the sonar (N/S/E/W): ";
            cin >> dir;
            dir = toupper(dir);
        }
        // Creates packet with necessary information to send to server (action - 1 because enums start at 0)
        PDataPacket packet = new DataPacket(client, static_cast<Operation>(action - 1), cI,
            treasureNearby, trapNearby, sonar,
            dir, player.getEnergy(), x, y,
            game.getCurrentTurn(), game.getTurnLimit(), treasuresFound,
            game.getIsRunning(), game.getPlayer().getPosition(), false);
        // Allocates for response
        PDataPacket response = new DataPacket();
        sendtorecvfromMsg(s, &server_addr, packet, response, prefix);
        if (action == 1) // Sets player position if action is move
        {
            player.setPosition(response->position);
        }
        // Sets all other values
        player.setEnergy(response->energy);
        game.setPlayer(player);
        game.setCurrentTurn(response->currentTurn);
        treasureNearby = response->treasureNearby;
        trapNearby = response->trapNearby;
        sonar = response->sonar;
        isRunning = response->isRunning;
        Map map = game.getMap();
        Cell cell = map.getCell(player.getPosition().x, player.getPosition().y);
		switch (action)
		{
            // Message for when player inspects the cell he's standing on
		    case 2:
		    {
			    if (response->isDug)
			    {
				    cout << "Cell has already been dug." << endl;
			    }
                if (response->cellInfo == FLAG)
                {
                    cout << "There's a flag here." << endl;
                    break;
                }
			    if (!response->isDug)
			    {
				    cout << "Cell hasn't been dug." << endl;
			    }
		    }
		    break;

            // When player digs cell
            case 3:
            {
                // Receives cellInfo value
                cI = response->cellInfo;
                // Changes boolean
                cell.isDug = response->isDug;

                // Sets cell in the copied map
                map.setCell(player.getPosition().x, player.getPosition().y, cell);

                // Sets map in the game
                game.setMap(map);

                // Prints something depending on the information
                switch (cI)
                {
                case NOTHING:
                {
                    cout << "There's nothing here." << endl;
                }
                break;
                case TREASURE:
                {
                    cout << "Treasure found!" << endl;
                    game.setTreasuresFound(response->treasuresFound);
                }
                break;
                case TRAP:
                {
                    cout << "It's a trap!" << endl;
                }
                break;
                }
            }
            break;

            // Messages for when the player uses the map
            case 4:
            {
                if (trapNearby && treasureNearby)
                {
                    cout << "Be careful! There are traps and treasures nearby!" << endl;
                }
                else if (trapNearby)
                {
                    cout << "Danger! Trap nearby!" << endl;
                }
                else if(treasureNearby)
                {
                    cout << "Treasure nearby! Keep looking." << endl;
                }
                else
                {
                    cout << "No signal of treasures nor traps nearby." << endl;
                }
            }
            break;

            // Flags the cell where the player is currently standing on
            case 5:
            {
                if (response->cellInfo == FLAG)
                {
                    cell.hasFlag = true;
                    map.setCell(player.getPosition().x, player.getPosition().y, cell);
                    game.setMap(map);
                }
            }
            break;

            // Eats
            case 6:
            {
                cout << "You replenished your energy. Current energy: " << player.getEnergy() << endl;
            }
            break;

            // Uses sonar
            case 7:
            {
                if (sonar)
                {
                    cout << "The sonar detects something in that direction." << endl;
                }
                else
                {
                    cout << "Nothing detected in that direction." << endl;
                }
            }
            break;
		}
        clearScreen();
    }
    // Shows the amount of treasures player has revealed
    game.getMap().displayMap(player.getPosition());
    cout << "Game finished. Treasures found: " << treasuresFound << endl;
    std::cout << "Client finishing..." << std::endl;
    int iResult = closesocket(s);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"closesocket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    WSACleanup();
    return 0;
}

//sends first msg to server and returns with the new server_addr used for the server for the dedicated socket
int obtainNewPort(SOCKET s, sockaddr_in* server_addr, string prefix) {
    PDataPacket packet = new DataPacket(); //I don't initialize because the server won't care
    PDataPacket response = new DataPacket();
    //IMPORTANT: will overwrite server_addr with the server addr with the new port, since the response msg in the server is sent through the new socket!
    sendtorecvfromMsg(s, server_addr, packet, response, prefix);
    return 0;
}

