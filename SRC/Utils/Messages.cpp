#include <utils/Messages.h>
#include <SDL2/SDL.h>

void LogErrorAndExit(const char* msg, const char* error)
{
    // Log the error message
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}
