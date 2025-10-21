#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class GameGrid;

#include "SnakeSprite.hpp"

class Snake {
public:
  enum class Direction { Up, Down, Left, Right };

  explicit Snake(sf::Vector2i startPosition, int initialLength = 3);

  void move();
  void setDirection(Direction newDirection);
  Direction getDirection() const { return currentDirection; }

  void grow();
  void setGrowthEnabled(bool enabled);
  bool isGrowthEnabled() const { return growthEnabled; }

  const std::vector<sf::Vector2i>& getBody() const { return body; }
  sf::Vector2i getHead() const;
  sf::Vector2i getTail() const;
  int getLength() const { return static_cast<int>(body.size()); }

  bool checkSelfCollision() const;
  bool checkWallCollision(int gridWidth, int gridHeight) const;
  bool checkCollisionWithPosition(sf::Vector2i position) const;
  bool checkCollisionWithWalls(const std::vector<sf::Vector2i>& wallPositions) const;

  bool isAlive() const { return alive; }
  bool isBlinking() const { return blinking; }
  void kill() {
    alive = false;
    tongueVisible = false;
  }
  void reset(sf::Vector2i startPosition, int initialLength = 3);

  void setDisoriented(bool disoriented, float duration = 0.0f);
  void setInvincible(bool invincible, float duration = 0.0f);
  void setSpeedMultiplier(float multiplier, float duration = 0.0f);
  void cancelInvincibility();
  void cancelDisorientation();
  bool isDisoriented() const { return disoriented; }
  bool isInvincible() const { return invincible; }
  float getSpeedMultiplier() const { return speedMultiplier; }

  void render(sf::RenderWindow& window, const GameGrid& grid) const;
  void setSnakeType(SnakeSprite::SnakeType type);
  SnakeSprite::SnakeType getSnakeType() const;
  float getSpeed() const { return speed; }
  void setSpeed(float speed) { this->speed = speed; }
  void decreaseSpeed(float amount);
  void setBlinking(bool blinking) { this->blinking = blinking; }

private:
  std::vector<sf::Vector2i> body;
  Direction currentDirection;
  Direction nextDirection;
  bool alive;
  bool directionChanged;
  bool growthEnabled;
  SnakeSprite snakeSprite;
  float speed;
  mutable sf::Sprite tongueSprite;
  bool blinking = false;
  mutable sf::Clock blinkTimer;

  bool disoriented = false;
  bool invincible = false;
  float speedMultiplier = 1.0f;
  sf::Clock disorientedTimer;
  sf::Clock invincibleTimer;
  sf::Clock speedMultiplierTimer;
  float disorientedDuration = 0.0f;
  float invincibleDuration = 0.0f;
  float speedMultiplierDuration = 0.0f;

  SnakeSprite::SnakeType originalSnakeType = SnakeSprite::SnakeType::Purple;
  bool hasTemporaryType = false;

  float temporarySpeedBonus = 0.0f;
  float temporarySpeedDuration = 0.0f;
  sf::Clock temporarySpeedTimer;

  float fantomSpeedBonus = 0.0f;
  float fantomSpeedDuration = 0.0f;
  sf::Clock fantomSpeedTimer;

  sf::Clock automaticSpeedTimer;
  static constexpr float AUTOMATIC_SPEED_INTERVAL = 5.0f;

  mutable float tongueTimer;
  mutable bool tongueVisible = false;
  static constexpr float TONGUE_DURATION = 0.5f;

  void updateDirection();
  void updateEffects();
  sf::Vector2i getNextHeadPosition() const;
  float getDirectionRotation() const;
  float getBodySegmentRotation(int segmentIndex) const;
  float getBodyCornerRotation(int segmentIndex) const;
  float getTailRotation() const;
  SnakeSprite::SegmentType getSegmentType(int segmentIndex) const;
  bool isBodyCorner(int segmentIndex) const;

  void updateTongue(const GameGrid& grid) const;
};
