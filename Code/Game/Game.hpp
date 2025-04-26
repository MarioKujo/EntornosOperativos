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
    /**
     * @brief The game map, representing the playing field.
     */
    Map map;

    /**
     * @brief The player character within the game.
     */
    Player player;

    /**
     * @brief Maximum number of turns allowed in the game.
     */
    int turnLimit;

    /**
     * @brief The current turn number in the game.
     */
    int currentTurn;

    /**
     * @brief Flag indicating whether the game is still running.
     */
    bool isRunning;

    /**
     * @brief Number of treasures the player has found so far.
     */
    int treasuresFound;

    /**
     * @brief Total number of treasures in the game.
     */
    int maxTreasures;

public:
    /**
     * @brief Constructor with map size and turn limit.
     * @param width Width of the game map.
     * @param height Height of the game map.
     * @param turns Maximum number of turns.
     */
    Game(int width, int height, int turns);

    /**
     * @brief Constructor with map size, number of treasures, and turn limit.
     * @param width Width of the game map.
     * @param height Height of the game map.
     * @param numTreasures Number of treasures to place on the map.
     * @param turnLimit Maximum number of turns.
     */
    Game(int width, int height, int numTreasures, int turnLimit);

    /**
     * @brief Default constructor that initializes an empty game.
     */
    Game();

    /**
     * @brief Main game loop that runs each turn.
     * Displays game status and prompts for player input.
     */
    void run();

    /**
     * @brief Displays the menu of available actions the player can take.
     */
    void showMenu();

    /**
     * @brief Returns the turn limit of the game.
     * @return The maximum number of turns.
     */
    int getTurnLimit();

    /**
     * @brief Returns the current turn number.
     * @return The current turn.
     */
    int getCurrentTurn();

    /**
     * @brief Returns whether the game is still running or not.
     * @return True if the game is running, false otherwise.
     */
    bool getIsRunning();

    /**
     * @brief Returns the player object.
     * @return The player character.
     */
    Player getPlayer();

    /**
     * @brief Returns the map object.
     * @return The game map.
     */
    Map getMap();

    /**
     * @brief Sets the current turn number.
     * @param newTurn The new turn number.
     */
    void setCurrentTurn(int newTurn);

    /**
     * @brief Sets the player object to a new value.
     * @param newVal The new player object.
     */
    void setPlayer(Player newVal);

    /**
     * @brief Sets the map object to a new value.
     * @param newMap The new map object.
     */
    void setMap(Map newMap);

    /**
     * @brief Sets the number of treasures found by the player.
     * @param newVal The new number of treasures found.
     */
    void setTreasuresFound(int newVal);
};
