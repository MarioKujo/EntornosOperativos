#pragma once
#include <iostream>
#include <string>
using namespace std;

class Cell
{
public:
    bool hasTreasure = false;  // Indicates whether the cell contains a treasure
    bool hasTrap = false;      // Indicates whether the cell contains a trap
    bool isDug = false;        // True if the cell has been dug by the player
    bool hasFlag = false;      // True if the player has placed a flag on the cell
};
