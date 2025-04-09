#include "Map.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

// Constructor that initializes the map with given dimensions and number of treasures
Map::Map(int w, int h, int numTreasures) : width(w), height(h)
{
    grid.resize(height, vector<Cell>(width));  // Resize the grid to match the map size
    srand((unsigned int)time(0));  // Seed the random number generator

    // Place treasures randomly on the map
    while (numTreasures > 0)
    {
        int x = rand() % width;  // Random x coordinate
        int y = rand() % height; // Random y coordinate
        if (!grid[y][x].hasTreasure)  // Check if the cell doesn't already have a treasure
        {
            grid[y][x].hasTreasure = true;  // Place treasure in the cell
            numTreasures--;  // Decrease the number of treasures to place
        }
    }
}

// Constructor that initializes the map with given dimensions, without treasures
Map::Map(int w, int h) : width(w), height(h)
{
    grid.resize(height, vector<Cell>(width));  // Resize the grid to match the map size
}

// Default constructor that initializes the map with no dimensions
Map::Map() : height(0), width(0) {}


// Displays the map with the player's position, flags, and dug cells
void Map::displayMap(const Position& playerPos) const
{
    // Loop through each row (y-axis)
    for (int y = 0; y < height; ++y)
    {
        // Loop through each column (x-axis)
        for (int x = 0; x < width; ++x)
        {
            // Display player's position as 'P'
            if (playerPos.x == x && playerPos.y == y)
            {
                cout << "P ";
            }
            // Display flag as 'F'
            else if (grid[y][x].hasFlag)
            {
                cout << "F ";
            }
            // Display dug cells as '*'
            else if (grid[y][x].isDug)
            {
                cout << "* ";
            }
            // Display undiscovered cells as '.'
            else
            {
                cout << ". ";
            }
        }
        cout << endl;
    }
}

// Returns a reference to the cell at the given coordinates
Cell& Map::getCell(int x, int y)
{
    return grid[y][x];
}

// Checks if the given position (x, y) is within valid map bounds
bool Map::isValidPosition(int x, int y) const
{
    return x >= 0 && x < width && y >= 0 && y < height;  // Ensure position is within map's width and height
}

// Sets the given cell at coordinates (x, y) to a new cell
void Map::setCell(int x, int y, Cell newCell)
{
    grid[y][x] = newCell;  // Update the cell at the specified position
}

// Returns the height (rows) of the map
int Map::getHeight()
{
    return height;
}

// Returns the width (columns) of the map
int Map::getWidth()
{
    return width;
}
