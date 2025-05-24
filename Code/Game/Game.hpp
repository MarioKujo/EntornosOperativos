#pragma once
#include <iostream>
#include "Map.hpp"
#include "Player.hpp"

/**
 * @class Game
 * @brief Represents the game itself, managing the main loop, player, and map.
 *
 * This class handles the game logic, such as running the main game loop,
 * displaying the map, tracking turns, managing the player's actions,
 * and controlling the state of the game.
 */
class Game
{
private:
    Map map;                   ///< The game map, representing the playing field.
    Player player;             ///< The player character within the game.
    int turnLimit;             ///< Maximum number of turns allowed in the game.
    int currentTurn;           ///< The current turn number in the game.
    bool isRunning;            ///< Flag indicating whether the game is still running.
    int treasuresFound;        ///< Number of treasures the player has found so far.
    int maxTreasures;          ///< Total number of treasures in the game.

public:
    // Constructors
    Game(int width, int height, int turns);
    Game(int width, int height, int numTreasures, int turnLimit);
    Game();

    // Main game loop that runs each turn
    void run();

    // Displays the menu of available actions the player can take
    void showMenu();

    // Getters
    int getTurnLimit() const;
    int getCurrentTurn() const;
    bool getIsRunning() const;
    Player& getPlayer();
    Map& getMap();

    // Setters
    void setCurrentTurn(int newTurn);
    void setTreasuresFound(int newVal);
};
