#include "Game.hpp"
#include <iostream>

// Constructor with map size and turn limit
Game::Game(int width, int height, int turns) : map(width, height), turnLimit(turns), currentTurn(0), isRunning(true) {}

// Constructor with map size, number of treasures, and turn limit
Game::Game(int width, int height, int numTreasures, int limit)
	: map(width, height, numTreasures), turnLimit(limit), currentTurn(0), isRunning(true) {}

// Default constructor
Game::Game() :map(0, 0, 0), turnLimit(0), currentTurn(0), isRunning(false) {}


// Main game loop that runs each turn
void Game::run()
{
	cout << "Turn: " << currentTurn + 1 << "/" << turnLimit << endl;  // Display current turn
	cout << "Energy: " << player.getEnergy() << endl;  // Display current player energy
	map.displayMap(player.getPosition());  // Display the map with the player's current position
	showMenu();  // Show the available menu options
}

// Displays the menu of available actions
void Game::showMenu()
{
	cout << "\nActions:" << endl;
	cout << "1. Move\n2. Inspect cell\n3. Dig\n4. Use map\n5. Flag\n6. Eat\n7. Use sonar\n8. Exit\nChoose an option: ";
}

// Processes the chosen action based on user input
void Game::processAction(int action)
{
	switch (action)
	{
	case 4:  // Use map
	{
		player.useMap(map);
	}
	break;
	case 5:  // Place flag
	{
		player.placeFlag(map);
	}
	break;
	case 6:  // Eat to replenish energy
	{
		player.eat();
	}
	break;
	case 7:  // Use sonar to detect nearby objects
	{
		player.useSonar(map);
	}
	break;
	case 8:  // Exit the game
	{
		isRunning = false;
	}
	break;
	default:  // Invalid option
	{
		cout << "Invalid option." << endl;
	}
	break;
	}
}

// Returns the turn limit
int Game::getTurnLimit()
{
	return turnLimit;
}

// Returns the current turn number
int Game::getCurrentTurn()
{
	return currentTurn;
}

// Returns whether the game is still running or not
bool Game::getIsRunning()
{
	return isRunning;
}

// Returns the player object
Player Game::getPlayer()
{
	return player;
}

// Returns the map object
Map Game::getMap()
{
	return map;
}

// Sets the current turn to a new value
void Game::setCurrentTurn(int newTurn)
{
	currentTurn = newTurn;
}

// Sets the player object to a new value
void Game::setPlayer(Player newVal)
{
	player = newVal;
}

// Sets the map object to a new value
void Game::setMap(Map newMap)
{
	map = newMap;
}
