#pragma once
#include <SFML/Graphics.hpp>

class SnakeSprite {
public:
  enum class SnakeType { Purple, Green, Blue, Red, Black };

  enum class SegmentType { Head, Body, BodyCorner, Tail };

  explicit SnakeSprite(SnakeType type = SnakeType::Green);

  sf::Sprite getHeadSprite() const;
  sf::Sprite getBodySprite() const;
  sf::Sprite getBodyCornerSprite() const;
  sf::Sprite getTailSprite() const;

  sf::Sprite getHeadSprite(float rotation) const;
  sf::Sprite getBodySprite(float rotation) const;
  sf::Sprite getBodyCornerSprite(float rotation) const;
  sf::Sprite getTailSprite(float rotation) const;

  sf::Sprite getTongueLowSprite() const;
  sf::Sprite getTongueHighSprite() const;
  sf::Sprite getTongueLowSprite(float rotation) const;
  sf::Sprite getTongueHighSprite(float rotation) const;

  void setType(SnakeType type);
  SnakeType getType() const;

private:
  SnakeType currentType;
  sf::Texture texture;

  static constexpr int SPRITE_WIDTH = 28;
  static constexpr int SPRITE_HEIGHT = 28;
  static constexpr int SHEET_WIDTH = 252;
  static constexpr int SHEET_HEIGHT = 140;

  static constexpr int BODY_CORNER_Y = 0;
  static constexpr int TONGUE_Y = 28;
  static constexpr int HEAD_Y = 56;
  static constexpr int BODY_Y = 84;
  static constexpr int TAIL_Y = 112;
  static constexpr int STATE_MARGIN = 56;

  static constexpr int SNAKE_TONGUE_LOW_X = 56;
  static constexpr int SNAKE_TONGUE_LOW_Y = TONGUE_Y;
  static constexpr int SNAKE_TONGUE_HIGH_X = 112;
  static constexpr int SNAKE_TONGUE_HIGH_Y = TONGUE_Y;

  sf::IntRect getSpriteRect(SegmentType segment) const;
  sf::IntRect getTongueSpriteRect(int tongueType) const;

  bool loadTexture();
};
