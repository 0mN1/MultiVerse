#include "GameHandler.hpp"

int main()
{
//    cout << "Main is ON\n";

    GameHandler Game;

    //Game.CharacterSelectionLoop();

    if(Game.GameOn)
    Game.GameLoop();

    return 0;
}
