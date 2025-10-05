#pragma once
#include <SFML/Graphics.hpp>

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

  // Get tongue sprites
  sf::Sprite getTongueLowSprite() const;
  sf::Sprite getTongueHighSprite() const;
  sf::Sprite getTongueLowSprite(float rotation) const;
  sf::Sprite getTongueHighSprite(float rotation) const;

  // Set snake type
  void setType(SnakeType type);
  SnakeType getType() const;

private:
  SnakeType currentType;
  sf::Texture texture;

  // Sprite sheet dimensions
  static constexpr int SPRITE_WIDTH = 28;
  static constexpr int SPRITE_HEIGHT = 28;
  static constexpr int SHEET_WIDTH = 252;
  static constexpr int SHEET_HEIGHT = 140;

  // Sprite positions in the sheet
  static constexpr int BODY_CORNER_Y = 0;  // Row 1: y=0-27
  static constexpr int TONGUE_Y = 28;      // Row 2: y=28-55
  static constexpr int HEAD_Y = 56;        // Row 3: y=56-83
  static constexpr int BODY_Y = 84;        // Row 4: y=84-111
  static constexpr int TAIL_Y = 112;       // Row 5: y=112-139
  static constexpr int STATE_MARGIN = 56;  // Fixed: 252/5 = 50.4, but with 28px sprites: (252-5*28)/4 = 28px margin

  static constexpr int SNAKE_TONGUE_LOW_X = 56;
  static constexpr int SNAKE_TONGUE_LOW_Y = TONGUE_Y;
  static constexpr int SNAKE_TONGUE_HIGH_X = 112;
  static constexpr int SNAKE_TONGUE_HIGH_Y = TONGUE_Y;

  // Calculate sprite position for given type and segment
  sf::IntRect getSpriteRect(SegmentType segment) const;
  sf::IntRect getTongueSpriteRect(int tongueType) const;

  // Load texture
  bool loadTexture();
};
