#include "Game.hpp"
#include <iostream>
Game::Game(int width, int height, int turns) : map(width, height), turnLimit(turns), currentTurn(0), isRunning(true)
{
}
Game::Game(int width, int height, int numTreasures, int limit)
	: map(width, height, numTreasures), turnLimit(limit), currentTurn(0), isRunning(true) {}

Game::Game() :map(0, 0, 0), turnLimit(0), currentTurn(0), isRunning(false)
{
}

void Game::run()
{
	cout << "Turn: " << currentTurn + 1 << "/" << turnLimit << endl;
	cout << "Energy: " << player.getEnergy() << endl;
	map.displayMap(player.getPosition());
	showMenu();
}

void Game::showMenu()
{
	cout << "\nActions:" << endl;
	cout << "1. Move\n2. Inspect cell\n3. Dig\n4. Use map\n5. Flag\n6. Eat\n7. Exit\nChoose an option: ";
}

void Game::processAction(int action)
{
	switch (action)
	{
		case 1:
		{
			int dx, dy;
			cout << "Movement (insert 2 separate numbers: x movement and y movement): ";
			cin >> dx >> dy;
			player.move(dx, dy, map);
			break;
		}
		case 2:
		{
			player.inspect(map);
			break;
		}
		case 3:
		{
			player.dig(map);
			break;
		}
		case 4:
		{
			player.useMap(map);
			break;
		}
		case 5:
		{
			player.placeFlag(map);
			break;
		}
		case 6:
		{
			player.eat();
			break;
		}
		case 7:
		{
			isRunning = false;
			break;
		}
		default:
		{
			cout << "Invalid option." << endl;
			break;
		}
	}
}

int Game::getTurnLimit()
{
	return turnLimit;
}

int Game::getCurrentTurn()
{
	return currentTurn;
}

bool Game::getIsRunning()
{
	return isRunning;
}

Player Game::getPlayer()
{
	return player;
}

Map Game::getMap()
{
	return map;
}

void Game::setPlayer(Player newVal)
{
	player = newVal;
}
