#include "Game.hpp"
#include <iostream>

// Constructor with map size and turn limit
Game::Game(int width, int height, int turns)
    : map(width, height),
    turnLimit(turns),
    currentTurn(1),
    isRunning(true),
    treasuresFound(0),
    maxTreasures(5)
{}

// Constructor with map size, number of treasures, and turn limit
Game::Game(int width, int height, int numTreasures, int limit)
    : map(width, height, numTreasures),
    turnLimit(limit),
    currentTurn(1),
    isRunning(true),
    treasuresFound(0),
    maxTreasures(numTreasures)
{}

// Default constructor
Game::Game()
    : map(0, 0, 0),
    turnLimit(0),
    currentTurn(1),
    isRunning(false),
    treasuresFound(0),
    maxTreasures(0)
{}

// Main game loop that runs each turn
void Game::run()
{
    cout << "Turn: " << currentTurn << "/" << turnLimit << endl;
    cout << "Energy: " << player.getEnergy() << endl;
    cout << "Treasures found: " << treasuresFound << "/" << maxTreasures << endl;
    map.displayMap(player.getPosition());
    showMenu();
}

// Displays the menu of available actions
void Game::showMenu()
{
    cout << "\nActions:\n"
        << "1. Move\n"
        << "2. Inspect cell\n"
        << "3. Dig\n"
        << "4. Use map\n"
        << "5. Flag\n"
        << "6. Eat\n"
        << "7. Use sonar\n"
        << "8. Exit\n"
        << "Choose an option: ";
}

// Getters
int Game::getTurnLimit() const
{
    return turnLimit;
}

int Game::getCurrentTurn() const
{
    return currentTurn;
}

bool Game::getIsRunning() const
{
    return isRunning;
}

Player Game::getPlayer() const
{
    return player;
}

Map Game::getMap() const
{
    return map;
}

// Setters
void Game::setCurrentTurn(int newTurn)
{
    currentTurn = newTurn;
}

void Game::setPlayer(const Player& newVal)
{
    player = newVal;
}

void Game::setMap(const Map& newMap)
{
    map = newMap;
}

void Game::setTreasuresFound(int newVal)
{
    treasuresFound = newVal;
}
