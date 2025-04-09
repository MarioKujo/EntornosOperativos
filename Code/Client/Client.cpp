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
    enum Operation ops = MOVE;
    Game game(10, 10, 30);
    game.run();
    while (game.getIsRunning() && game.getCurrentTurn() < game.getTurnLimit() && (game.getPlayer().getEnergy() > 0))
    {
        system("CLS");
        game.run();
        game.showMenu();
        cout << "Before cin>>action" << endl;
        int action;
        cin >> action;
        cout << "After cin>>action" << endl;
        action--;
        int x = 0, y = 0;
        if (action == 0)
        {
            cout << "Introduce el desplazamiento de x: ";
            cin >> x;
            cout << "Introduce el desplazamiento de y: ";
            cin >> y;
        }
        PDataPacket packet = new DataPacket(client, static_cast<Operation>(action), x, y, game.getCurrentTurn(), game.getIsRunning(), game.getPlayer().getPosition());
        PDataPacket response = new DataPacket();
        sendtorecvfromMsg(s, &server_addr, packet, response, prefix);
        Player player;
        player.setPosition(response->position);
        game.setPlayer(player);
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

