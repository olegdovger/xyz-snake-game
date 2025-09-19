#pragma once
#include <SFML/Graphics.hpp>

namespace utils {

class SnakeSprite {
public:
  enum class SnakeType { Purple, Green, Blue, Red, Black };

  enum class SegmentType { Head, Body, BodyCorner, Tail };

  explicit SnakeSprite(SnakeType type = SnakeType::Green);

  // Get sprite for specific segment
  sf::Sprite getHeadSprite() const;
  sf::Sprite getBodySprite() const;
  sf::Sprite getBodyCornerSprite() const;
  sf::Sprite getTailSprite() const;

  // Get sprite with rotation for direction
  sf::Sprite getHeadSprite(float rotation) const;
  sf::Sprite getBodySprite(float rotation) const;
  sf::Sprite getBodyCornerSprite(float rotation) const;
  sf::Sprite getTailSprite(float rotation) const;

  // Set snake type
  void setSnakeType(SnakeType type);

private:
  SnakeType currentType;
  sf::Texture texture;

  // Sprite sheet dimensions
  static constexpr int SPRITE_WIDTH = 28;
  static constexpr int SPRITE_HEIGHT = 28;
  static constexpr int SHEET_WIDTH = 252;
  static constexpr int SHEET_HEIGHT = 140;

  // Sprite positions in the sheet
  static constexpr int HEAD_Y = 64;
  static constexpr int BODY_Y = 64 + 28;
  static constexpr int BODY_CORNER_Y = 0;
  static constexpr int TAIL_Y = 64 + 28 + 28;
  static constexpr int STATE_MARGIN = 112;

  // Calculate sprite position for given type and segment
  sf::IntRect getSpriteRect(SegmentType segment) const;

  // Load texture
  bool loadTexture();
};

}  // namespace utils
