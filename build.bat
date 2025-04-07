@echo off
echo Building...

:: Compile with explicitly listed files
g++ -I./include ^
    -I./lib/SDL2/include ^
    -I./lib/SDL2/include/SDL2 ^
    -I./lib/SDL2_image/include ^
    -I./lib/SDL2_mixer/include ^
    -I./lib/SDL2_ttf/include ^
    -L./lib/SDL2/lib ^
    -L./lib/SDL2_image/lib ^
    -L./lib/SDL2_mixer/lib ^
    -L./lib/SDL2_ttf/lib ^
    main.cpp ^
    src/entities/base/Entity.cpp ^
    src/entities/base/Collider.cpp ^
    src/entities/base/Movement.cpp ^
    src/entities/base/Rotation.cpp ^
    src/entities/base/CollisionHandler.cpp ^
    src/entities/enemy/Enemy.cpp ^
    src/entities/enemy/EnemySpawner.cpp ^
    src/entities/Player.cpp ^
    src/states/PlayingState.cpp ^
    src/states/MenuState.cpp ^
    src/states/QuitState.cpp ^
    src/states/State.cpp ^
    src/core/Game.cpp ^
    src/core/Renderer.cpp ^
    src/core/SoundManager.cpp ^
    src/core/Window.cpp ^
    -o main.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

:: Check for build errors
if %errorlevel% neq 0 (
    echo Build failed.
    exit /b %errorlevel%
)

echo Build complete. 