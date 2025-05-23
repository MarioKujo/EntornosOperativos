#include "Player.hpp"
#include <iostream>

Player::Player() : pos{ 0,0 }, energy(100), treasuresFound(0), info{ 0, 0 } {}
// Constructor initializing position, energy, treasures found, treasures nearby and trap nearby

Position Player::getPosition() const
{
    return pos;
}  // Returns the current position of the player

// Moves the player by a specified delta (dx, dy) if the new position is valid
bool Player::move(int dx, int dy, const Map& map)
{
    Position newPos = { pos.x + dx, pos.y + dy };
    if (map.isValidPosition(newPos.x, newPos.y))
    {
        pos = newPos;
        energy -= 5;
        return true;
    }
    return false;
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

// Inspect the cell at the player's current position
InspectInfo Player::inspect(const Map& map) const
{
    const Cell& cell = map.getCell(pos.x, pos.y);
    return { cell.isDug, cell.hasFlag };
}

// Places a flag on the cell at the player's current position
void Player::placeFlag(Map& map)
{
    Cell& cell = map.getCell(pos.x, pos.y);  // Access the cell at the player's position
    cell.hasFlag = true;  // Mark the cell as flagged
}

// Uses the map to check the area around the player for treasures or traps
NearbyInfo Player::useMap(const Map& map) const
{
    NearbyInfo info
    {
        false, false
    };
    int range = 2;

    for (int dx = -range; dx <= range; ++dx)
    {
        for (int dy = -range; dy <= range; ++dy)
        {
            int newX = pos.x + dx;
            int newY = pos.y + dy;
            if (map.isValidPosition(newX, newY))
            {
                const Cell& cell = map.getCell(newX, newY);
                if (cell.hasTreasure)
                {
                    info.treasureNearby = true;
                }

                if (cell.hasTrap) 
                {
                    info.trapNearby = true;
                }
            }
        }
    }
    return info;
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
int Player::getEnergy() const
{
    return energy;
}
int Player::getTreasuresFound() const
{
    return treasuresFound;
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
