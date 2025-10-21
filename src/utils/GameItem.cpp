#include "GameItem.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "ResourceLoader.hpp"

GameItem::GameItem(sf::Vector2i position, float lifetime)
    : position(position),
      lifetime(lifetime),
      remainingTime(lifetime),
      expired(false),
      sprite(ResourceLoader::getTexture(TextureType::GreenApple)) {}

bool GameItem::update(float deltaTime) {
  if (expired) {
    return false;
  }

  remainingTime -= deltaTime;
  if (remainingTime <= 0.0f) {
    expired = true;
    return false;
  }

  return true;
}

bool GameItem::checkCollision(sf::Vector2i position) const {
  return this->position == position;
}

unsigned char GameItem::getAlpha() const {
  if (expired || lifetime <= 0.0f) {
    return 0;
  }

  float lifetimePercent = remainingTime / lifetime;

  if (lifetimePercent > 0.05f) {
    float alphaPercent = (lifetimePercent - 0.05f) / 0.95f;
    return static_cast<unsigned char>(100 + (255 - 100) * alphaPercent);
  } else {
    float alphaPercent = lifetimePercent / 0.05f;
    return static_cast<unsigned char>(100 * alphaPercent);
  }
}
