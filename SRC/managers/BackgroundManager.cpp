#include "managers/BackgroundManager.h"

BackgroundManager::BackgroundManager(SDL_Renderer* renderer, const SDL_Texture* texture, int width, int height)
  : m_renderer(renderer), m_backgroundTexture(const_cast<SDL_Texture*>(texture)), m_width(width), m_height(height),
    m_scrollOffset(0.0f), m_scrollSpeed(100.0f), m_lastUpdateTime(SDL_GetTicks()) {
}

BackgroundManager::~BackgroundManager() {
  // Note: We don't destroy the texture here as it might be managed elsewhere
}

void BackgroundManager::RenderBackground() {
  if (m_backgroundTexture) {
    SDL_Rect destRect = { 0, 0, m_width, m_height };
    SDL_RenderCopy(m_renderer, m_backgroundTexture, NULL, &destRect);
  }
}

void BackgroundManager::SetBackgroundTexture(SDL_Texture* texture) {
  m_backgroundTexture = texture;
}

SDL_Texture* BackgroundManager::GetBackgroundTexture() const {
  return m_backgroundTexture;
}

void BackgroundManager::SetScrollSpeed(float speed) {
  m_scrollSpeed = speed;
}

float BackgroundManager::GetScrollSpeed() const {
  return m_scrollSpeed;
}

void BackgroundManager::InfiniteBackground() {
  // Implementation for vertical scrolling background effect
  if (m_backgroundTexture) {
    // Get current time and calculate delta time
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - m_lastUpdateTime) / 1000.0f;
    m_lastUpdateTime = currentTime;
    
    // Get the original texture dimensions
    int textureWidth, textureHeight;
    SDL_QueryTexture(m_backgroundTexture, NULL, NULL, &textureWidth, &textureHeight);
    
    // Update scroll offset
    m_scrollOffset += m_scrollSpeed * deltaTime;
    
    // Wrap the scroll offset when it exceeds texture height
    while (m_scrollOffset > textureHeight) {
      m_scrollOffset -= textureHeight;
    }
    while (m_scrollOffset < -textureHeight) {
      m_scrollOffset += textureHeight;
    }
    
    // Calculate how many times to tile the texture
    int horizontalTiles = (m_width + textureWidth - 1) / textureWidth;
    int verticalTiles = (m_height + textureHeight - 1) / textureHeight + 1; // +1 for smooth scrolling
    
    for (int y = 0; y < verticalTiles; ++y) {
      for (int x = 0; x < horizontalTiles; ++x) {
        SDL_Rect destRect = { 
          x * textureWidth, 
          static_cast<int>(y * textureHeight - m_scrollOffset), 
          textureWidth, 
          textureHeight 
        };
        SDL_RenderCopy(m_renderer, m_backgroundTexture, NULL, &destRect);
      }
    }
  }
}