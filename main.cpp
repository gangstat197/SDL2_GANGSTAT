#include <SDL2/SDL.h> 
#include <core/Game.h>
#include <systems/InputSystem.h>
#include <iostream>
#include <utils/GameSettings.h>

int main(int argc, char* argv[]) { 
    Game game;
    if (!game.Init(GameSettings::GAME_TITLE, 
                  GameSettings::SCREEN_WIDTH, 
                  GameSettings::SCREEN_HEIGHT)) {
        std::cout << "Failed to init game.\n";
        return -1;
    }
    
    std::cout << "Game initialized successfully" << std::endl;

    while (game.IsRunning()) {
        game.HandleEvents();
        game.Update();
        game.Render();   
    }

    game.Clean();
    return 0;
}
