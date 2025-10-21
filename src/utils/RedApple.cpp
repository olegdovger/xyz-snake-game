#include "RedApple.hpp"
#include "../Snake.hpp"
#include "GameGrid.hpp"
#include "ResourceLoader.hpp"

RedApple::RedApple(sf::Vector2i position, int boardWidth, int boardHeight, float snakeSpeed, float lifetimeMultiplier)
    : GameItem(position, calculateLifetime(boardWidth, boardHeight, snakeSpeed) * lifetimeMultiplier) {
  sprite = sf::Sprite(ResourceLoader::getTexture(TextureType::RedApple));

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

  unsigned char alpha = getAlpha();
  const_cast<sf::Sprite&>(sprite).setColor(sf::Color(255, 255, 255, alpha));

  window.draw(sprite);
}

void RedApple::applySpecialEffects(Snake& snake) const {
  snake.decreaseSpeed(1.0f);

  snake.cancelInvincibility();
  snake.cancelDisorientation();
}

float RedApple::calculateLifetime(int boardWidth, int boardHeight, float snakeSpeed) {
  float horizontalTime = static_cast<float>(boardWidth) / snakeSpeed;
  float verticalTime = static_cast<float>(boardHeight) / snakeSpeed;

  return std::max(horizontalTime, verticalTime);
}
