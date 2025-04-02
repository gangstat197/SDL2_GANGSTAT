# Define paths to the unified SDL directories
SDL_DIR = ./lib

SOURCES = main.cpp $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)

# Default target - just builds SDL2
all:
	g++ -I ./include \
		-I $(SDL_DIR)/SDL2/include \
		-I $(SDL_DIR)/SDL2/include/SDL2 \
		-I $(SDL_DIR)/SDL2_image/include \
		-I $(SDL_DIR)/SDL2_mixer/include \
		-I $(SDL_DIR)/SDL2_ttf/include \
		-L $(SDL_DIR)/SDL2/lib \
		-L $(SDL_DIR)/SDL2_image/lib \
		-L $(SDL_DIR)/SDL2_mixer/lib \
		-L $(SDL_DIR)/SDL2_ttf/lib \
		$(SOURCES) \
		-o main -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
	@echo "Build complete."

