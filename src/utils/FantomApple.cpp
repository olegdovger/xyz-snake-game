#include "FantomApple.hpp"
#include "../Snake.hpp"
#include "GameGrid.hpp"
#include "ResourceLoader.hpp"

FantomApple::FantomApple(sf::Vector2i position, float lifetimeMultiplier)
    : GameItem(position, 12.0f * lifetimeMultiplier) {

  sprite.setTexture(ResourceLoader::getTexture(TextureType::FantomApple));

  float scale = 32.0f / static_cast<float>(sprite.getTexture().getSize().x);
  sprite.setScale(sf::Vector2f(scale, scale));
}

void FantomApple::render(sf::RenderWindow& window, const GameGrid& grid) const {
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

void FantomApple::applySpecialEffects(Snake& snake) const {
  snake.setInvincible(true, INVINCIBILITY_DURATION);
  snake.setSpeedMultiplier(1.0f, 0.0f);

  snake.setSnakeType(SnakeSprite::SnakeType::Black);

  snake.cancelDisorientation();
}
