#pragma once
#include <iostream>
#include "Map.hpp"
#include "Player.hpp"
class Game
{
private:
    Map map;
    Player player;
    int turnLimit;
    int currentTurn;
    bool isRunning;

public:
    Game(int width, int height, int numTreasures, int turnLimit);
    void run();
    void showMenu();
    void processAction(int action);
    void clearScreen();
};