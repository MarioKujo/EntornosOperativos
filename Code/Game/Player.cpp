#include "Player.hpp"
#include <iostream>

Player::Player() : pos{ 0, 0 }, energy(100), treasuresFound(0) {}  // Constructor initializing position, energy, and treasures found

Position Player::getPosition() const { return pos; }  // Returns the current position of the player

// Moves the player by a specified delta (dx, dy) if the new position is valid
void Player::move(int dx, int dy, const Map& map)
{
    Position newPos = { pos.x + dx, pos.y + dy };
    if (map.isValidPosition(newPos.x, newPos.y))  // Check if the new position is valid on the map
    {
        pos = newPos;
        energy -= 5;  // Decrease energy on movement
    }
}

// Digs the cell at the player's current position, revealing treasure or not
void Player::dig(Map& map)
{
    Cell& cell = map.getCell(pos.x, pos.y);  // Access the cell at the player's position
    cell.isDug = true;  // Mark the cell as dug
    energy -= 10;  // Decrease energy for digging
    if (cell.hasTreasure)  // If the cell contains treasure, increment the treasure count
    {
        treasuresFound++;
    }
}

// Inspect the cell at the player's current position (doesn't currently do anything)
void Player::inspect(Map& map) const
{
    const Cell& cell = map.getCell(pos.x, pos.y);  // Access the cell, but no action is taken
}

// Places a flag on the cell at the player's current position
void Player::placeFlag(Map& map)
{
    Cell& cell = map.getCell(pos.x, pos.y);  // Access the cell at the player's position
    cell.hasFlag = true;  // Mark the cell as flagged
}

// Uses the map to check the area around the player for treasures or traps
void Player::useMap(Map& map)
{
    int range = 2;  // Range of cells to check around the player
    treasureNearby = false;
    trapNearby = false;

    // Checks cells around the player within the range
    for (int dx = -range; dx <= range; ++dx)
    {
        for (int dy = -range; dy <= range; ++dy)
        {
            int newX = pos.x + dx;
            int newY = pos.y + dy;

            // Check if the cell is within the map bounds
            if (map.isValidPosition(newX, newY))
            {
                const Cell& cell = map.getCell(newX, newY);

                // Check for treasures or traps
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
}

// Restores energy to the player by 20 units
void Player::eat()
{
    energy += 20;  // Increase energy
}

// Uses sonar to detect treasures or traps in a specified direction
bool Player::useSonar(Map& map, char dir, int x, int y) const
{
    bool found = false;

    // Check in the specified direction (N, S, E, W) for treasures or traps
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
    }
    return found;
}

// Getter methods for player energy and treasures found
int Player::getEnergy() const { return energy; }
int Player::getTreasuresFound() const { return treasuresFound; }

bool Player::getTreasureNearby()
{
    return treasureNearby;
}

bool Player::getTrapNearby()
{
    return trapNearby;
}

// Setter methods for position and energy
void Player::setPosition(Position newPosition)
{
    pos = newPosition;  // Update the player's position
}

void Player::setEnergy(int newEnergy)
{
    energy = newEnergy;  // Set the player's energy to a new value
}
