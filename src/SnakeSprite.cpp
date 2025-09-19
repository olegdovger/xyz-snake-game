#include "SnakeSprite.hpp"
#include "utils/ResourceLoader.hpp"

namespace utils {

SnakeSprite::SnakeSprite(SnakeType type) : currentType(type) {
  loadTexture();
}

sf::Sprite SnakeSprite::getHeadSprite() const {
  sf::Sprite sprite(texture);
  sprite.setTextureRect(getSpriteRect(SegmentType::Head));
  return sprite;
}

sf::Sprite SnakeSprite::getBodySprite() const {
  sf::Sprite sprite(texture);
  sprite.setTextureRect(getSpriteRect(SegmentType::Body));
  return sprite;
}

sf::Sprite SnakeSprite::getTailSprite() const {
  sf::Sprite sprite(texture);
  sprite.setTextureRect(getSpriteRect(SegmentType::Tail));
  return sprite;
}

sf::Sprite SnakeSprite::getHeadSprite(float rotation) const {
  sf::Sprite sprite = getHeadSprite();
  sprite.setRotation(sf::degrees(rotation));
  sprite.setOrigin(sf::Vector2f(SPRITE_WIDTH / 2.0f, SPRITE_HEIGHT / 2.0f));
  return sprite;
}

sf::Sprite SnakeSprite::getBodySprite(float rotation) const {
  sf::Sprite sprite = getBodySprite();
  sprite.setRotation(sf::degrees(rotation));
  sprite.setOrigin(sf::Vector2f(SPRITE_WIDTH / 2.0f, SPRITE_HEIGHT / 2.0f));
  return sprite;
}

sf::Sprite SnakeSprite::getTailSprite(float rotation) const {
  sf::Sprite sprite = getTailSprite();
  sprite.setRotation(sf::degrees(rotation));
  sprite.setOrigin(sf::Vector2f(SPRITE_WIDTH / 2.0f, SPRITE_HEIGHT / 2.0f));
  return sprite;
}

void SnakeSprite::setSnakeType(SnakeType type) {
  currentType = type;
}

sf::IntRect SnakeSprite::getSpriteRect(SegmentType segment) const {
  int x = static_cast<int>(currentType) * STATE_MARGIN;
  int y = 0;

  switch (segment) {
    case SegmentType::Head:
      y = HEAD_Y;
      break;
    case SegmentType::Body:
      y = BODY_Y;
      break;
    case SegmentType::Tail:
      y = TAIL_Y;
      break;
  }

  return sf::IntRect(sf::Vector2i(x, y), sf::Vector2i(SPRITE_WIDTH, SPRITE_HEIGHT));
}

bool SnakeSprite::loadTexture() {
  const auto& textureRef = ResourceLoader::getTexture(TextureType::Snake);
  texture = textureRef;
  return true;
}

}  // namespace utils
