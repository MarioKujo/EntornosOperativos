#pragma once
#include <iostream>
#include <vector>
#include "Cell.hpp"
#include "Position.hpp"
class Map
{
private:
    vector<vector<Cell>> grid;
    int width, height;
public:
    Map(int width, int height, int numTreasures);
    Map(int width, int height);
    Map();
    void displayMap(const Position& playerPos) const;
    Cell& getCell(int x, int y);
    bool isValidPosition(int x, int y) const;

    void setCell(int x, int y, Cell newCell);

    int getHeight();
    int getWidth();
};
