#pragma once
#include "Map.hpp"
#include "Position.hpp"

class Player
{
private:
    Position pos;              // Player's position on the map
    int energy;                // Player's energy level
    int treasuresFound;        // Number of treasures the player has found
    bool treasureNearby;
    bool trapNearby;
public:
    Player();                  // Constructor to initialize the player

    void move(int dx, int dy, const Map& map);  // Moves the player by dx, dy if the position is valid
    void dig(Map& map);        // Digs the current cell, revealing treasure or not
    void inspect(Map& map) const;  // Inspects the current cell (currently does nothing)
    void placeFlag(Map& map);  // Places a flag on the current cell
    void useMap(Map& map); // Uses the map to check nearby cells for treasures or traps
    void eat();                // Restores the player's energy
    void useSonar(Map& map) const;  // Uses sonar to check for treasures or traps in a specific direction

    Position getPosition() const;  // Returns the player's current position
    int getEnergy() const;         // Returns the player's current energy level
    int getTreasuresFound() const; // Returns the number of treasures the player has found
    bool getTreasureNearby();
    bool getTrapNearby();

    void setPosition(Position newPosition);  // Sets the player's position to a new value
    void setEnergy(int newEnergy);           // Sets the player's energy to a new value
};
