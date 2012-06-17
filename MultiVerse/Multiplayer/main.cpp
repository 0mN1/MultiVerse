#include "GameHandler.hpp"

int main()
{
    GameHandler Game;

    if(Game.GameOn)
    Game.CharacterSelectionLoop();

    if(Game.GameOn)
    Game.NetworkOptionsLoop();

    Game.SaveSettings();

    if(Game.GameOn)
    {
        if(Game.isServer)
        Game.ServerLoop();
        else
        Game.ClientLoop();
    }

    return 0;
}
