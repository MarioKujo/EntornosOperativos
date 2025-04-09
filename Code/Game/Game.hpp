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
    Game(int width, int height, int turns);
    Game(int width, int height, int numTreasures, int turnLimit);
    Game();
    void run();
    void showMenu();
    void processAction(int action);
    int getTurnLimit();
    int getCurrentTurn();
    bool getIsRunning();
    Player getPlayer();
    Map getMap();

    void setCurrentTurn(int newTurn);
    void setPlayer(Player newVal);
    void setMap(Map newMap);
};