#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include "Cell.hpp"
#include "Position.hpp"

/**
 * @class Map
 * @brief Represents the game map composed of a 2D grid of cells.
 *
 * This class manages the grid structure, map dimensions,
 * and provides methods to interact with and modify the map.
 */
class Map
{
private:
    /**
     * @brief 2D grid representing the map of cells.
     */
    vector<vector<Cell>> grid;

    /**
     * @brief Width of the map (number of columns).
     */
    int width;

    /**
     * @brief Height of the map (number of rows).
     */
    int height;

public:
    /**
     * @brief Constructs a map with specified dimensions and a given number of treasures.
     * @param width Width of the map.
     * @param height Height of the map.
     * @param numTreasures Number of treasures to randomly place on the map.
     */
    Map(int width, int height, int numTreasures);

    /**
     * @brief Constructs a map with specified dimensions without treasures.
     * @param width Width of the map.
     * @param height Height of the map.
     */
    Map(int width, int height);

    /**
     * @brief Default constructor that initializes an empty map.
     */
    Map();

    /**
     * @brief Displays the map, highlighting the player's current position.
     * @param playerPos The current position of the player.
     */
    void displayMap(const Position& playerPos) const;

    /**
     * @brief Returns a reference to the cell at the specified coordinates.
     * @param x Horizontal coordinate (column index).
     * @param y Vertical coordinate (row index).
     * @return Reference to the corresponding Cell.
     */
    const Cell& getCell(int x, int y) const;
    Cell& getCell(int x, int y);

    /**
     * @brief Checks if the given coordinates are valid within the map bounds.
     * @param x Horizontal coordinate (column index).
     * @param y Vertical coordinate (row index).
     * @return True if the position is valid, false otherwise.
     */
    bool isValidPosition(int x, int y) const;

    /**
     * @brief Sets a new cell at the specified coordinates.
     * @param x Horizontal coordinate (column index).
     * @param y Vertical coordinate (row index).
     * @param newCell The new Cell to set.
     */
    void setCell(int x, int y, const Cell& newCell);

    /**
     * @brief Returns the height of the map (number of rows).
     * @return Height of the map.
     */
    int getHeight() const;

    /**
     * @brief Returns the width of the map (number of columns).
     * @return Width of the map.
     */
    int getWidth() const;
};
