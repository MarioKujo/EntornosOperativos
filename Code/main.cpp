#include <iostream>
#include "Game.hpp"
int main()
{
    Game game(10, 10, 5, 50);  // Mapa de 10x10, 5 tesoros, 50 turnos
    game.run();
    return 0;
}
