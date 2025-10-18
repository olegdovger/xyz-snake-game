#include "RedApple.hpp"
#include "../Snake.hpp"
#include "GameGrid.hpp"
#include "ResourceLoader.hpp"

RedApple::RedApple(sf::Vector2i position, int boardWidth, int boardHeight, float snakeSpeed, float lifetimeMultiplier)
    : GameItem(position, calculateLifetime(boardWidth, boardHeight, snakeSpeed) * lifetimeMultiplier) {
  sprite = sf::Sprite(ResourceLoader::getTexture(TextureType::RedApple));

  // Scale sprite to fit grid cell (assuming 32x32 grid cells)
  float scale = 32.0f / static_cast<float>(sprite.getTexture().getSize().x);
  sprite.setScale(sf::Vector2f(scale, scale));
}

void RedApple::render(sf::RenderWindow& window, const GameGrid& grid) const {
  if (expired)
    return;

  sf::Vector2f worldPos = grid.getCellPosition(position.y, position.x);
  const_cast<sf::Sprite&>(sprite).setPosition(worldPos);
  const auto scale = grid.getScaledCellSize() / static_cast<float>(sprite.getTexture().getSize().x);
  const_cast<sf::Sprite&>(sprite).setScale(sf::Vector2f(scale, scale));

  // Set alpha based on remaining lifetime
  unsigned char alpha = getAlpha();
  const_cast<sf::Sprite&>(sprite).setColor(sf::Color(255, 255, 255, alpha));

  window.draw(sprite);
}

void RedApple::applySpecialEffects(Snake& snake) const {
  // Decrease speed by 1
  snake.decreaseSpeed(1.0f);

  snake.cancelInvincibility();
  snake.cancelDisorientation();
}

float RedApple::calculateLifetime(int boardWidth, int boardHeight, float snakeSpeed) {
  // Calculate time to traverse board horizontally or vertically
  float horizontalTime = static_cast<float>(boardWidth) / snakeSpeed;
  float verticalTime = static_cast<float>(boardHeight) / snakeSpeed;

  // Use the longer traversal time as lifetime
  return std::max(horizontalTime, verticalTime);
}
