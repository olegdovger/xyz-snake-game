#include "Snake.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include "SnakeSprite.hpp"
#include "utils/GameGrid.hpp"

Snake::Snake(sf::Vector2i startPosition, int initialLength)
    : currentDirection(Direction::Right),
      nextDirection(Direction::Right),
      alive(true),
      directionChanged(false),
      growthEnabled(false),
      snakeSprite(SnakeSprite::SnakeType::Green),
      speed(1.0f),
      tongueSprite(snakeSprite.getTongueLowSprite()),
      tongueTimer(0.0f),
      tongueVisible(false) {

  body.reserve(initialLength);
  for (int i = 0; i < initialLength; ++i) {
    body.push_back(sf::Vector2i(startPosition.x - i, startPosition.y));
  }
}

void Snake::move() {
  if (!alive)
    return;

  updateEffects();

  updateDirection();

  sf::Vector2i newHead = getNextHeadPosition();

  body.insert(body.begin(), newHead);

  if (!growthEnabled) {
    body.pop_back();
  }

  growthEnabled = false;
  directionChanged = false;
}

void Snake::setDirection(Direction newDirection) {
  if (disoriented) {
    switch (newDirection) {
      case Direction::Left:
        newDirection = Direction::Right;
        break;
      case Direction::Right:
        newDirection = Direction::Left;
        break;
      case Direction::Up:
        newDirection = Direction::Down;
        break;
      case Direction::Down:
        newDirection = Direction::Up;
        break;
    }
  }

  if (currentDirection == Direction::Up && newDirection == Direction::Down)
    return;
  if (currentDirection == Direction::Down && newDirection == Direction::Up)
    return;
  if (currentDirection == Direction::Left && newDirection == Direction::Right)
    return;
  if (currentDirection == Direction::Right && newDirection == Direction::Left)
    return;

  nextDirection = newDirection;
  directionChanged = true;
}

void Snake::grow() {
  growthEnabled = true;
}

void Snake::setGrowthEnabled(bool enabled) {
  growthEnabled = enabled;
}

sf::Vector2i Snake::getHead() const {
  return body.empty() ? sf::Vector2i(0, 0) : body[0];
}

sf::Vector2i Snake::getTail() const {
  return body.empty() ? sf::Vector2i(0, 0) : body.back();
}

bool Snake::checkSelfCollision() const {
  if (invincible) {
    return false;
  }

  if (body.size() < 3)
    return false;

  sf::Vector2i head = getHead();
  return std::find(body.begin() + 1, body.end(), head) != body.end();
}

bool Snake::checkWallCollision(int gridWidth, int gridHeight) const {
  if (invincible) {
    sf::Vector2i head = getHead();
    return head.x < 0 || head.x >= gridWidth || head.y < 0 || head.y >= gridHeight;
  }

  sf::Vector2i head = getHead();
  return head.x < 0 || head.x >= gridWidth || head.y < 0 || head.y >= gridHeight;
}

bool Snake::checkCollisionWithPosition(sf::Vector2i position) const {
  return std::find(body.begin(), body.end(), position) != body.end();
}

bool Snake::checkCollisionWithWalls(const std::vector<sf::Vector2i>& wallPositions) const {
  if (invincible) {
    return false;
  }

  for (const auto& wallPos : wallPositions) {
    if (std::find(body.begin(), body.end(), wallPos) != body.end()) {
      return true;
    }
  }
  return false;
}

void Snake::reset(sf::Vector2i startPosition, int initialLength) {
  body.clear();
  body.reserve(initialLength);
  for (int i = 0; i < initialLength; ++i) {
    body.push_back(sf::Vector2i(startPosition.x - i, startPosition.y));
  }
  currentDirection = Direction::Right;
  nextDirection = Direction::Right;
  alive = true;
  directionChanged = false;
  growthEnabled = false;

  snakeSprite.setType(SnakeSprite::SnakeType::Purple);
  hasTemporaryType = false;

  temporarySpeedBonus = 0.0f;
  temporarySpeedDuration = 0.0f;

  fantomSpeedBonus = 0.0f;
  fantomSpeedDuration = 0.0f;
}

void Snake::setSnakeType(SnakeSprite::SnakeType type) {
  snakeSprite.setType(type);
  hasTemporaryType = (type != SnakeSprite::SnakeType::Purple);
}

void Snake::render(sf::RenderWindow& window, const GameGrid& grid) const {
  if (!isAlive() && !isBlinking()) {
    return;
  }

  for (size_t i = 0; i < body.size(); ++i) {
    SnakeSprite::SegmentType segmentType = getSegmentType(static_cast<int>(i));
    sf::Sprite segment = [&]() -> sf::Sprite {
      switch (segmentType) {
        case SnakeSprite::SegmentType::Head:
          return snakeSprite.getHeadSprite(getDirectionRotation());
        case SnakeSprite::SegmentType::Body:
          return snakeSprite.getBodySprite(getBodySegmentRotation(static_cast<int>(i)));
        case SnakeSprite::SegmentType::BodyCorner:
          return snakeSprite.getBodyCornerSprite(getBodyCornerRotation(static_cast<int>(i)));
        case SnakeSprite::SegmentType::Tail:
          return snakeSprite.getTailSprite(getTailRotation());
        default:
          return snakeSprite.getBodySprite();
      }
    }();

    sf::Vector2f position = grid.getCellPosition(body[i].y, body[i].x);
    sf::Vector2f centerOffset(grid.getScaledCellSize() / 2.0f, grid.getScaledCellSize() / 2.0f);
    segment.setPosition(position + centerOffset);

    float scale = grid.getScaledCellSize() / 28.0f;
    segment.setScale(sf::Vector2f(scale, scale));

    if (blinking) {
      float time = blinkTimer.getElapsedTime().asSeconds();
      float alpha = 128 + 127 * std::sin(time * 3.14159f * 4.0f);
      segment.setColor(sf::Color(255, 255, 255, static_cast<unsigned char>(alpha)));

    } else {
      segment.setColor(sf::Color::White);
    }

    window.draw(segment);
  }

  updateTongue(grid);

  if (tongueVisible && isAlive()) {
    sf::Vector2f headPosition = grid.getCellPosition(body[0].y, body[0].x);
    sf::Vector2f centerOffset(grid.getScaledCellSize() / 2.0f, grid.getScaledCellSize() / 2.0f);

    sf::Vector2f tongueOffset(0.0f, 0.0f);
    switch (currentDirection) {
      case Direction::Up:
        tongueOffset.y = -grid.getScaledCellSize() * 1.5f;
        break;
      case Direction::Down:
        tongueOffset.y = grid.getScaledCellSize() * 1.5f;
        break;
      case Direction::Left:
        tongueOffset.x = -grid.getScaledCellSize() * 1.5f;
        break;
      case Direction::Right:
        tongueOffset.x = grid.getScaledCellSize() * 1.5f;
        break;
    }

    tongueSprite.setPosition(headPosition + centerOffset + tongueOffset);

    float scale = grid.getScaledCellSize() / 28.0f;
    float tongueScale = scale * 2.0f;
    tongueSprite.setScale(sf::Vector2f(tongueScale, tongueScale));

    tongueSprite.setRotation(sf::degrees(getDirectionRotation()));

    window.draw(tongueSprite);
  }
}

void Snake::updateDirection() {
  currentDirection = nextDirection;
}

void Snake::updateTongue(const GameGrid& grid) const {
  if (!isAlive()) {
    tongueVisible = false;
    return;
  }

  if (tongueVisible) {
    tongueTimer += 1.0f / 60.0f;

    if (tongueTimer >= TONGUE_DURATION) {
      tongueVisible = false;
      tongueTimer = 0.0f;
    }

  } else {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    if (dis(gen) < 0.3f) {
      tongueVisible = true;
      tongueTimer = 0.0f;

      tongueSprite = (dis(gen) < 0.5f) ? snakeSprite.getTongueLowSprite(getDirectionRotation())
                                       : snakeSprite.getTongueHighSprite(getDirectionRotation());

      if (dis(gen) < 0.8f) {
        tongueSprite.setColor(sf::Color(255, 255, 255, 255));
      } else {
        tongueSprite.setColor(sf::Color(255, 255, 255, 0));
      }
    }
  }
}

sf::Vector2i Snake::getNextHeadPosition() const {
  sf::Vector2i head = getHead();

  switch (currentDirection) {
    case Direction::Up:
      return sf::Vector2i(head.x, head.y - 1);
    case Direction::Down:
      return sf::Vector2i(head.x, head.y + 1);
    case Direction::Left:
      return sf::Vector2i(head.x - 1, head.y);
    case Direction::Right:
      return sf::Vector2i(head.x + 1, head.y);
    default:
      return head;
  }
}

float Snake::getDirectionRotation() const {
  switch (currentDirection) {
    case Direction::Up:
      return 0.0f;
    case Direction::Right:
      return 90.0f;
    case Direction::Down:
      return 180.0f;
    case Direction::Left:
      return 270.0f;
    default:
      return 0.0f;
  }
}

float Snake::getBodySegmentRotation(int segmentIndex) const {
  if (segmentIndex >= static_cast<int>(body.size()) || segmentIndex < 1) {
    return 0.0f;
  }

  sf::Vector2i current = body[segmentIndex];
  sf::Vector2i previous = body[segmentIndex - 1];

  sf::Vector2i direction = current - previous;

  if (direction.x > 0) {
    return 90.0f;
  } else if (direction.x < 0) {
    return 270.0f;
  } else if (direction.y > 0) {
    return 180.0f;
  } else if (direction.y < 0) {
    return 0.0f;
  }

  return 0.0f;
}

float Snake::getBodyCornerRotation(int segmentIndex) const {
  if (segmentIndex <= 0 || segmentIndex >= static_cast<int>(body.size()) - 1) {
    return 0.0f;
  }

  sf::Vector2i prev = body[segmentIndex - 1];
  sf::Vector2i current = body[segmentIndex];
  sf::Vector2i next = body[segmentIndex + 1];

  sf::Vector2i dirFromPrev = current - prev;
  sf::Vector2i dirToNext = next - current;

  if (dirFromPrev.y < 0 && dirToNext.x > 0)
    return 0.0f;
  if (dirFromPrev.x > 0 && dirToNext.y > 0)
    return 90.0f;
  if (dirFromPrev.y > 0 && dirToNext.x < 0)
    return 180.0f;
  if (dirFromPrev.x < 0 && dirToNext.y < 0)
    return 270.0f;

  if (dirFromPrev.y < 0 && dirToNext.x < 0)
    return 90.0f;
  if (dirFromPrev.x < 0 && dirToNext.y > 0)
    return 0.0f;
  if (dirFromPrev.y > 0 && dirToNext.x > 0)
    return 270.0f;
  if (dirFromPrev.x > 0 && dirToNext.y < 0)
    return 180.0f;

  return 0.0f;
}

float Snake::getTailRotation() const {
  if (body.size() < 2) {
    return 0.0f;
  }

  sf::Vector2i tail = body.back();
  sf::Vector2i secondToLast = body[body.size() - 2];

  sf::Vector2i direction = tail - secondToLast;

  if (direction.x > 0) {
    return 270.0f;
  } else if (direction.x < 0) {
    return 90.0f;
  } else if (direction.y > 0) {
    return 0.0f;
  } else if (direction.y < 0) {
    return 180.0f;
  }

  return 0.0f;
}

SnakeSprite::SegmentType Snake::getSegmentType(int segmentIndex) const {
  if (segmentIndex == 0) {
    return SnakeSprite::SegmentType::Head;
  } else if (segmentIndex == static_cast<int>(body.size()) - 1) {
    return SnakeSprite::SegmentType::Tail;
  } else if (isBodyCorner(segmentIndex)) {
    return SnakeSprite::SegmentType::BodyCorner;
  } else {
    return SnakeSprite::SegmentType::Body;
  }
}

bool Snake::isBodyCorner(int segmentIndex) const {
  if (segmentIndex <= 0 || segmentIndex >= static_cast<int>(body.size()) - 1) {
    return false;
  }

  sf::Vector2i prev = body[segmentIndex - 1];
  sf::Vector2i current = body[segmentIndex];
  sf::Vector2i next = body[segmentIndex + 1];

  sf::Vector2i dirFromPrev = current - prev;
  sf::Vector2i dirToNext = next - current;

  return (dirFromPrev.x != dirToNext.x) || (dirFromPrev.y != dirToNext.y);
}

void Snake::setDisoriented(bool disoriented, float duration) {
  this->disoriented = disoriented;
  if (disoriented) {
    disorientedDuration = duration;
    disorientedTimer.restart();
  }
}

void Snake::setInvincible(bool invincible, float duration) {
  this->invincible = invincible;
  if (invincible) {
    invincibleDuration = duration;
    invincibleTimer.restart();
  } else {
    invincibleDuration = 0.0f;
    snakeSprite.setType(SnakeSprite::SnakeType::Purple);
    hasTemporaryType = false;
  }
}

void Snake::cancelInvincibility() {
  invincible = false;
  invincibleDuration = 0.0f;
  snakeSprite.setType(SnakeSprite::SnakeType::Purple);
  hasTemporaryType = false;
}

void Snake::cancelDisorientation() {
  disoriented = false;
  disorientedDuration = 0.0f;
  hasTemporaryType = false;
}

void Snake::setSpeedMultiplier(float multiplier, float duration) {
  this->speedMultiplier = multiplier;
  if (duration > 0.0f) {
    speedMultiplierDuration = duration;
    speedMultiplierTimer.restart();
  }
}

void Snake::decreaseSpeed(float amount) {
  speed = std::max(1.0f, speed - amount);
}

void Snake::updateEffects() {
  if (disoriented && disorientedDuration > 0.0f) {
    if (disorientedTimer.getElapsedTime().asSeconds() >= disorientedDuration) {
      disoriented = false;
      disorientedDuration = 0.0f;

      snakeSprite.setType(SnakeSprite::SnakeType::Purple);
      hasTemporaryType = false;
    }
  }

  if (invincible && invincibleDuration > 0.0f) {
    if (invincibleTimer.getElapsedTime().asSeconds() >= invincibleDuration) {
      invincible = false;
      invincibleDuration = 0.0f;

      snakeSprite.setType(SnakeSprite::SnakeType::Purple);
      hasTemporaryType = false;
    }
  }

  if (speedMultiplierDuration > 0.0f) {
    if (speedMultiplierTimer.getElapsedTime().asSeconds() >= speedMultiplierDuration) {
      speedMultiplier = 1.0f;
      speedMultiplierDuration = 0.0f;

      snakeSprite.setType(SnakeSprite::SnakeType::Purple);
      hasTemporaryType = false;
    }
  }

  if (temporarySpeedDuration > 0.0f) {
    if (temporarySpeedTimer.getElapsedTime().asSeconds() >= temporarySpeedDuration) {
      speed -= temporarySpeedBonus;
      temporarySpeedBonus = 0.0f;
      temporarySpeedDuration = 0.0f;

      snakeSprite.setType(SnakeSprite::SnakeType::Purple);
      hasTemporaryType = false;
    }
  }

  if (fantomSpeedDuration > 0.0f) {
    if (fantomSpeedTimer.getElapsedTime().asSeconds() >= fantomSpeedDuration) {
      speed -= fantomSpeedBonus;
      fantomSpeedBonus = 0.0f;
      fantomSpeedDuration = 0.0f;

      snakeSprite.setType(SnakeSprite::SnakeType::Purple);
      hasTemporaryType = false;
    }
  }

  if (automaticSpeedTimer.getElapsedTime().asSeconds() >= AUTOMATIC_SPEED_INTERVAL) {
    speed += 1.0f;
    automaticSpeedTimer.restart();
  }
}

SnakeSprite::SnakeType Snake::getSnakeType() const {
  return snakeSprite.getType();
}