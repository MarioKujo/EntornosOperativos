// Archivo: Cell.cpp
#include "Cell.hpp"

string Cell::inspect() const
{
    if (isDug)
    {
        if (hasTreasure)
        {
            return "Treasure found!";
        }
        if (hasTrap)
        {
            return "It's a trap!";
        }
        return "There's nothing here.";
    }
    else
    {
        return "Cell hasn't been dug.";
    }
}