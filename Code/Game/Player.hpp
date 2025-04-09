#pragma once
#include "Map.hpp"
#include "Position.hpp"
class Player
{
private:
    Position pos;
    int energy;
    int treasuresFound;

public:
    Player();
    void move(int dx, int dy, const Map& map);
    void dig(Map& map);
    void inspect(Map& map) const;
    void placeFlag(Map& map);
    void useMap(Map& map) const;
    void eat();
    void useSonar(Map& map)const;

    Position getPosition() const;
    int getEnergy() const;
    int getTreasuresFound() const;

    void setPosition(Position newPosition);
    void setEnergy(int newEnergy);
};
