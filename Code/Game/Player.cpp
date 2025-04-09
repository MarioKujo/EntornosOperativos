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
    cout << cell.inspect() << endl;
}

void Player::inspect(Map& map) const
{
    const Cell& cell = map.getCell(pos.x, pos.y);
    cout << cell.inspect() << endl;
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

int Player::getEnergy() const { return energy; }
int Player::getTreasuresFound() const { return treasuresFound; }

void Player::setPosition(Position newPosition)
{
    pos = newPosition;
}


