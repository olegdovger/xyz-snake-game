#include "GreenApple.hpp"
#include "../Snake.hpp"
#include "GameGrid.hpp"
#include "ResourceLoader.hpp"

GreenApple::GreenApple(sf::Vector2i position) : GameItem(position, 10.0f) {
  sprite = sf::Sprite(ResourceLoader::getTexture(TextureType::GreenApple));
  // Scale sprite to fit grid cell (assuming 32x32 grid cells)
  float scale = 32.0f / static_cast<float>(sprite.getTexture().getSize().x);
  sprite.setScale(sf::Vector2f(scale, scale));
}

void GreenApple::render(sf::RenderWindow& window, const GameGrid& grid) const {
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

void GreenApple::applySpecialEffects(Snake& snake) const {
  // Decrease speed by 2
  snake.decreaseSpeed(2.0f);

  snake.cancelInvincibility();
  snake.cancelDisorientation();
}
