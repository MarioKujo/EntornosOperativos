#include "Player.hpp"
#include <iostream>

Player::Player() : pos{ 0, 0 }, energy(100), treasuresFound(0) {}

Position Player::getPosition() const { return pos; }

void Player::move(int dx, int dy, const Map& map)
{
    Position newPos = { pos.x + dx, pos.y + dy };
    if (map.isValidPosition(newPos.x, newPos.y))
    {
        pos = newPos;
        energy -= 5;
        cout << "You have moved to (" << pos.x << ", " << pos.y << ")" << endl;
    }
    else
    {
        cout << "Invalid movement." << endl;
    }
}

void Player::dig(Map& map)
{
    Cell& cell = map.getCell(pos.x, pos.y);
    cell.isDug = true;
    energy -= 10;
    if (cell.hasTreasure)
    {
        treasuresFound++;
    }
}

void Player::inspect(Map& map) const
{
    const Cell& cell = map.getCell(pos.x, pos.y);
}

void Player::placeFlag(Map& map)
{
    Cell& cell = map.getCell(pos.x, pos.y);
    cell.hasFlag = true;
    cout << "You flagged this cell." << endl;
}

void Player::useMap(Map& map) const
{
    // Muestra la zona cercana (puedes cambiar el rango si lo prefieres)
    int range = 2;  // Rango de 2 celdas a la redonda
    bool treasureNearby = false;
    bool trapNearby = false;

    cout << "Searching..." << endl;

    // Recorre las celdas alrededor del jugador en el rango especificado
    for (int dx = -range; dx <= range; ++dx)
    {
        for (int dy = -range; dy <= range; ++dy)
        {
            int newX = pos.x + dx;
            int newY = pos.y + dy;

            // Verifica si la celda está dentro del mapa
            if (map.isValidPosition(newX, newY))
            {
                const Cell& cell = map.getCell(newX, newY);

                if (cell.hasTreasure)
                {
                    treasureNearby = true;
                }
                if (cell.hasTrap)
                {
                    trapNearby = true;
                }
            }
        }
    }

    // Da pistas basadas en la proximidad de tesoros o trampas
    if (treasureNearby && trapNearby)
    {
        cout << "Danger! Traps and treasures nearby." << endl;
    }
    else if (treasureNearby)
    {
        cout << "Treasure nearby! Keep looking." << endl;
    }
    else if (trapNearby)
    {
        cout << "Danger! Trap nearby!" << endl;
    }
    else
    {
        cout << "No signal of treasure nor traps nearby." << endl;
    }
}


void Player::eat()
{
    energy += 20;
    cout << "You replenished your energy. Current energy: " << energy << endl;
}

void Player::useSonar(Map& map) const
{
    std::cout << "Choose a direction to fire the sonar (N/S/E/W): ";
    char dir;
    std::cin >> dir;
    dir = std::toupper(dir);

    int x = pos.x;
    int y = pos.y;
    bool found = false;

    switch (dir)
    {
    case 'N':
        for (int i = y - 1; i >= 0; --i)
        {
            if (map.getCell(x, i).hasTreasure || map.getCell(x, i).hasTrap)
            {
                found = true;
                break;
            }
        }
        break;
    case 'S':
        for (int i = y + 1; i < map.getHeight(); ++i)
        {
            if (map.getCell(x, i).hasTreasure || map.getCell(x, i).hasTrap)
            {
                found = true;
                break;
            }
        }
        break;
    case 'E':
        for (int i = x + 1; i < map.getWidth(); ++i)
        {
            if (map.getCell(i, y).hasTreasure || map.getCell(i, y).hasTrap)
            {
                found = true;
                break;
            }
        }
        break;
    case 'W':
        for (int i = x - 1; i >= 0; --i)
        {
            if (map.getCell(i, y).hasTreasure || map.getCell(i, y).hasTrap)
            {
                found = true;
                break;
            }
        }
        break;
    default:
        cout << "Invalid direction." << endl;
        return;
    }

    if (found)
    {
        cout << "The sonar detects something in that direction." << endl;
    }
    else
    {
        cout << "Nothing detected in that direction." << endl;
    }

}

int Player::getEnergy() const { return energy; }
int Player::getTreasuresFound() const { return treasuresFound; }

void Player::setPosition(Position newPosition)
{
    pos = newPosition;
}

void Player::setEnergy(int newEnergy)
{
    energy = newEnergy;
}


