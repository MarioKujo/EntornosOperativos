#pragma once
#include "Map.hpp"
#include "Position.hpp"

/**
 * @class Player
 * @brief Represents the player character within the game.
 *
 * This class manages the player's state, including position, energy,
 * treasures found, and nearby hazards. It provides actions like moving,
 * digging, inspecting, flagging, and using sonar.
 */
class Player
{
private:
    /**
     * @brief Player's position on the map.
     */
    Position pos;

    /**
     * @brief Player's current energy level.
     */
    int energy;

    /**
     * @brief Number of treasures the player has found.
     */
    int treasuresFound;

    /**
     * @brief True if there is a treasure nearby the player.
     */
    bool treasureNearby;

    /**
     * @brief True if there is a trap nearby the player.
     */
    bool trapNearby;

public:
    /**
     * @brief Constructor to initialize the player with default values.
     */
    Player();

    /**
     * @brief Moves the player by the given deltas if the destination is valid.
     * @param dx Change in x-coordinate.
     * @param dy Change in y-coordinate.
     * @param map Reference to the current map.
     */
    void move(int dx, int dy, const Map& map);

    /**
     * @brief Digs at the player's current position to reveal treasures.
     * @param map Reference to the current map.
     */
    void dig(Map& map);

    /**
     * @brief Inspects the player's current cell (currently non-functional).
     * @param map Reference to the current map.
     */
    void inspect(Map& map) const;

    /**
     * @brief Places a flag at the player's current position.
     * @param map Reference to the current map.
     */
    void placeFlag(Map& map);

    /**
     * @brief Uses the map to scan nearby cells for treasures or traps.
     * @param map Reference to the current map.
     */
    void useMap(Map& map);

    /**
     * @brief Restores the player's energy by a fixed amount.
     */
    void eat();

    /**
     * @brief Uses sonar to detect treasures or traps in a specified direction.
     * @param map Reference to the current map.
     * @param dir Direction to scan ('N', 'S', 'E', 'W').
     * @param x Starting x-coordinate.
     * @param y Starting y-coordinate.
     * @return True if a treasure or trap is detected, false otherwise.
     */
    bool useSonar(Map& map, char dir, int x, int y) const;

    /**
     * @brief Returns the player's current position.
     * @return Current position of the player.
     */
    Position getPosition() const;

    /**
     * @brief Returns the player's current energy level.
     * @return Current energy level.
     */
    int getEnergy() const;

    /**
     * @brief Returns the number of treasures found by the player.
     * @return Number of treasures found.
     */
    int getTreasuresFound() const;

    /**
     * @brief Checks if a treasure is nearby.
     * @return True if a treasure is nearby, false otherwise.
     */
    bool getTreasureNearby();

    /**
     * @brief Checks if a trap is nearby.
     * @return True if a trap is nearby, false otherwise.
     */
    bool getTrapNearby();

    /**
     * @brief Sets the player's position to a new value.
     * @param newPosition New position to assign.
     */
    void setPosition(Position newPosition);

    /**
     * @brief Sets the player's energy to a new value.
     * @param newEnergy New energy level to assign.
     */
    void setEnergy(int newEnergy);
};
