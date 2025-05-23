#pragma once
#include <iostream>
#include <string>
using namespace std;

enum CellInfo
{
    NOTHING, TRAP, TREASURE, FLAG
};

/**
 * @class Cell
 * @brief Represents a single cell in the game grid.
 *
 * This class defines the properties of a cell, including
 * whether it contains a treasure, a trap, has been dug,
 * or has a flag placed by the player.
 */
class Cell
{
public:
    /**
     * @brief Indicates whether the cell contains a treasure.
     */
    bool hasTreasure = false;

    /**
     * @brief Indicates whether the cell contains a trap.
     */
    bool hasTrap = false;

    /**
     * @brief True if the cell has been dug by the player.
     */
    bool isDug = false;

    /**
     * @brief True if the player has placed a flag on the cell.
     */
    bool hasFlag = false;
};
