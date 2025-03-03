#include <Core/Game.h>
#include <Core/settings.h>
#include <iostream>

int main(int argc, char* argv[]) {
    Game game;

    if (!game.Init("Monster Trainer", SCREEN_WIDTH, SCREEN_HEIGHT)) {
        std::cout << "Failed to initialize game" << std::endl;
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