#pragma once
#include <iostream>
#include <vector>
#include "Cell.hpp"
#include "Position.hpp"

class Map
{
private:
    vector<vector<Cell>> grid;  // 2D grid representing the map of cells
    int width, height;          // Dimensions of the map (width and height)

public:
    Map(int width, int height, int numTreasures);  // Constructor with map size and number of treasures
    Map(int width, int height);                    // Constructor with map size only
    Map();                                         // Default constructor

    void displayMap(const Position& playerPos) const;  // Displays the map, highlighting the player's position
    Cell& getCell(int x, int y);                      // Returns a reference to the cell at coordinates (x, y)
    bool isValidPosition(int x, int y) const;          // Checks if the given coordinates are valid on the map

    void setCell(int x, int y, Cell newCell);          // Sets a new cell at the given coordinates

    int getHeight();  // Returns the height of the map
    int getWidth();   // Returns the width of the map
};
