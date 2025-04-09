#include "Map.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

Map::Map(int w, int h, int numTreasures) : width(w), height(h)
{
    grid.resize(height, vector<Cell>(width));
    srand((unsigned int)time(0));

    while (numTreasures > 0)
    {
        int x = rand() % width;
        int y = rand() % height;
        if (!grid[y][x].hasTreasure)
        {
            grid[y][x].hasTreasure = true;
            numTreasures--;
        }
    }
}

Map::Map(int w, int h) :width(w), height(h)
{
    grid.resize(height, vector<Cell>(width));
}

Map::Map(): height(0), width(0)
{
}

void Map::displayMap(const Position& playerPos) const
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (playerPos.x == x && playerPos.y == y)
            {
                cout << "P ";
            }
            else if (grid[y][x].hasFlag)
            {
                cout << "F ";
            }
            else if (grid[y][x].isDug)
            {
                cout << "* ";
            }
            else
            {
                cout << ". ";
            }
        }
        cout << endl;
    }
}

Cell& Map::getCell(int x, int y)
{
    return grid[y][x];
}

bool Map::isValidPosition(int x, int y) const
{
    return x >= 0 && x < width && y >= 0 && y < height;
}

int Map::getHeight()
{
    return height;
}

int Map::getWidth()
{
    return width;
}
