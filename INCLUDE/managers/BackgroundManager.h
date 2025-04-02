#include<SDL2/SDL.h>

class BackgroundManager {
  private :
    SDL_Texture* m_backgroundTexture;
    SDL_Renderer* m_renderer;
    int m_width;
    int m_height;

    float m_scrollOffset;
    float m_scrollSpeed;
    float m_lastUpdateTime;

  public : 
    BackgroundManager(SDL_Renderer* renderer, const SDL_Texture* texture, int width, int height);
    ~BackgroundManager();

    void RenderBackground();
    void SetBackgroundTexture(SDL_Texture* texture);
    SDL_Texture* GetBackgroundTexture() const;
    
    void InfiniteBackground();
    void SetScrollSpeed(float speed);
    float GetScrollSpeed() const;
};