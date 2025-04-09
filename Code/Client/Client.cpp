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
DWORD WINAPI CommunicationThread(LPVOID param);
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
    enum CellDug cd = NOTHING;
    Game game(10, 10, 30);
    game.run();
    system("CLS");
    bool isRunning = true;
    int treasuresFound = 0;
    Player player = game.getPlayer();
    bool treasureNearby = false, trapNearby = false;
    while (isRunning)
    {
        game.run();
        int action;
        cin >> action;
        int x = 0, y = 0;
        if (action == 1)
        {
            cout << "x displacement: ";
            cin >> x;
            cout << "y displacement: ";
            cin >> y;
        }
        PDataPacket packet = new DataPacket(client, static_cast<Operation>(action - 1), cd,
            treasureNearby, trapNearby,
            player.getEnergy(), x, y,
            game.getCurrentTurn(), game.getTurnLimit(), treasuresFound,
            game.getIsRunning(), game.getPlayer().getPosition(), false);
        PDataPacket response = new DataPacket();
        sendtorecvfromMsg(s, &server_addr, packet, response, prefix);
        if (action == 1)
        {
            player.setPosition(response->position);
        }
        player.setEnergy(response->energy);
        game.setPlayer(player);
        game.setCurrentTurn(response->currentTurn);
        treasureNearby = response->treasureNearby;
        trapNearby = response->trapNearby;
        Map map = game.getMap();
        Cell cell = map.getCell(player.getPosition().x, player.getPosition().y);
		switch (action)
		{
		    case 2:
		    {
			    if (response->isDug)
			    {
				    cout << "There's nothing here." << endl;
			    }
			    if (!response->isDug)
			    {
				    cout << "Cell hasn't been dug" << endl;
			    }
		    }
		    break;
            case 3:
            {
                cd = response->cellDug;
                cell.isDug = response->isDug;
                map.setCell(player.getPosition().x, player.getPosition().y, cell);
                game.setMap(map);
                switch (cd)
                {
                case NOTHING:
                {
                    cout << "There's nothing here." << endl;
                }
                break;
                case TREASURE:
                {
                    cout << "Treasure found!" << endl;
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
		}
    }
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

DWORD WINAPI CommunicationThread(LPVOID param)
{
    return 0;
}

