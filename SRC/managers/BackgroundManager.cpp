#include "managers/BackgroundManager.h"
#include <cmath>

BackgroundManager::BackgroundManager(SDL_Renderer* renderer, const SDL_Texture* texture, int width, int height)
  : m_renderer(renderer), m_backgroundTexture(const_cast<SDL_Texture*>(texture)), m_width(width), m_height(height),
    m_scrollOffset(0.0f), m_scrollSpeed(100.0f), m_lastUpdateTime(SDL_GetTicks()) {
}

BackgroundManager::~BackgroundManager() {
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
  if (!m_backgroundTexture) return;
  
  Uint32 currentTime = SDL_GetTicks();
  float deltaTime = (currentTime - m_lastUpdateTime) / 1000.0f;
  m_lastUpdateTime = currentTime;
  
  int textureWidth, textureHeight;
  SDL_QueryTexture(m_backgroundTexture, NULL, NULL, &textureWidth, &textureHeight);
  
  m_scrollOffset += m_scrollSpeed * deltaTime;
  
  m_scrollOffset = fmod(m_scrollOffset, static_cast<float>(textureHeight));
  
  int horizontalTiles = (m_width + textureWidth - 1) / textureWidth;
  int verticalTiles = (m_height + textureHeight - 1) / textureHeight + 1;
  
  float firstRowY = m_scrollOffset - textureHeight;
  
  for (int y = 0; y <= verticalTiles; ++y) {
    float yPos = firstRowY + (y * textureHeight);
    
    for (int x = 0; x < horizontalTiles; ++x) {
      if (yPos < -textureHeight || yPos > m_height) {
        continue;
      }
      
      SDL_Rect destRect = { 
        x * textureWidth,
        static_cast<int>(yPos),
        textureWidth, 
        textureHeight 
      };
      
      SDL_RenderCopy(m_renderer, m_backgroundTexture, NULL, &destRect);
    }
  }
}