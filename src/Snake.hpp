#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

namespace utils {
class GameGrid;
}

#include "SnakeSprite.hpp"

class Snake {
public:
  enum class Direction { Up, Down, Left, Right };

  explicit Snake(sf::Vector2i startPosition, int initialLength = 3);

  // Movement
  void move();
  void setDirection(Direction newDirection);
  Direction getDirection() const { return currentDirection; }

  // Growth
  void grow();
  void shrink();
  void setGrowthEnabled(bool enabled);
  bool isGrowthEnabled() const { return growthEnabled; }

  // Position and body
  const std::vector<sf::Vector2i>& getBody() const { return body; }
  sf::Vector2i getHead() const;
  sf::Vector2i getTail() const;
  int getLength() const { return static_cast<int>(body.size()); }

  // Collision detection
  bool checkSelfCollision() const;
  bool checkWallCollision(int gridWidth, int gridHeight) const;
  bool checkCollisionWithPosition(sf::Vector2i position) const;

  // State
  bool isAlive() const { return alive; }
  bool isBlinking() const { return blinking; }
  void kill() {
    alive = false;
    tongueVisible = false;
  }
  void reset(sf::Vector2i startPosition, int initialLength = 3);

  // Rendering
  void render(sf::RenderWindow& window, const utils::GameGrid& grid) const;
  void setSnakeType(utils::SnakeSprite::SnakeType type);
  float getSpeed() const { return speed; }
  void setSpeed(float speed) { this->speed = speed; }
  void setBlinking(bool blinking) { this->blinking = blinking; }
  void setBlinkTimer(const sf::Clock& timer) { blinkTimer = timer; }

private:
  std::vector<sf::Vector2i> body;
  Direction currentDirection;
  Direction nextDirection;
  bool alive;
  bool directionChanged;
  bool growthEnabled;
  utils::SnakeSprite snakeSprite;
  float speed;
  mutable sf::Sprite tongueSprite;
  bool blinking = false;
  mutable sf::Clock blinkTimer;

  mutable float tongueTimer;
  mutable bool tongueVisible = false;
  static constexpr float TONGUE_DURATION = 0.5f;  // 0.3 seconds

  void updateDirection();
  sf::Vector2i getNextHeadPosition() const;
  float getDirectionRotation() const;
  float getBodySegmentRotation(int segmentIndex) const;
  float getBodyCornerRotation(int segmentIndex) const;
  float getTailRotation() const;
  utils::SnakeSprite::SegmentType getSegmentType(int segmentIndex) const;
  bool isBodyCorner(int segmentIndex) const;

  void updateTongue(const utils::GameGrid& grid) const;
};
