#pragma once
#include <iostream>
#include "Map.hpp"
#include "Player.hpp"

class Game
{
private:
    Map map;                  // The game map
    Player player;            // The player in the game
    int turnLimit;            // Maximum number of turns in the game
    int currentTurn;          // Current turn number
    bool isRunning;           // Whether the game is still running or not

public:
    Game(int width, int height, int turns);                      // Constructor with map size and turn limit
    Game(int width, int height, int numTreasures, int turnLimit); // Constructor with map size, treasures, and turn limit
    Game();                                                       // Default constructor

    void run();                     // Main game loop that runs each turn
    void showMenu();                // Displays the menu of available actions
    void processAction(int action); // Processes the chosen action
    int getTurnLimit();             // Returns the turn limit
    int getCurrentTurn();           // Returns the current turn number
    bool getIsRunning();            // Returns whether the game is running or not
    Player getPlayer();             // Returns the player object
    Map getMap();                   // Returns the map object

    void setCurrentTurn(int newTurn);   // Sets a new value for the current turn
    void setPlayer(Player newVal);      // Sets the player object to a new value
    void setMap(Map newMap);            // Sets the map object to a new value
};
