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
      snakeSprite(utils::SnakeSprite::SnakeType::Green),
      speed(1.0f),
      tongueSprite(snakeSprite.getTongueLowSprite()),
      tongueTimer(0.0f),
      tongueVisible(false) {
  // Initialize body starting from head
  body.reserve(initialLength);
  for (int i = 0; i < initialLength; ++i) {
    body.push_back(sf::Vector2i(startPosition.x - i, startPosition.y));
  }
}

void Snake::move() {
  if (!alive)
    return;

  updateDirection();

  // Get next head position
  sf::Vector2i newHead = getNextHeadPosition();

  // Add new head
  body.insert(body.begin(), newHead);

  // Remove tail (unless growing)
  if (!growthEnabled) {
    body.pop_back();
  }

  // Reset growth flag after move
  growthEnabled = false;
  directionChanged = false;
}

void Snake::setDirection(Direction newDirection) {
  // Prevent reversing into itself
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
  // Snake grows by not removing tail on next move
  growthEnabled = true;
}

void Snake::shrink() {
  if (body.size() > 1) {
    body.pop_back();
  }
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
  if (body.size() < 3)
    return false;  // Need at least 3 segments to collide with self

  sf::Vector2i head = getHead();
  return std::find(body.begin() + 1, body.end(), head) != body.end();
}

bool Snake::checkWallCollision(int gridWidth, int gridHeight) const {
  sf::Vector2i head = getHead();
  return head.x < 0 || head.x >= gridWidth || head.y < 0 || head.y >= gridHeight;
}

bool Snake::checkCollisionWithPosition(sf::Vector2i position) const {
  return std::find(body.begin(), body.end(), position) != body.end();
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
}

void Snake::setSnakeType(utils::SnakeSprite::SnakeType type) {
  snakeSprite.setSnakeType(type);
}

void Snake::render(sf::RenderWindow& window, const utils::GameGrid& grid) const {
  // Always render if blinking, otherwise only if alive
  if (!isAlive() && !isBlinking()) {
    std::cout << "Snake not rendering: alive=" << isAlive() << ", blinking=" << isBlinking() << std::endl;
    return;
  }

  if (isBlinking()) {
    std::cout << "Snake rendering while blinking: alive=" << isAlive() << ", body size=" << body.size() << std::endl;
  }

  for (size_t i = 0; i < body.size(); ++i) {
    // Get sprite for this segment
    utils::SnakeSprite::SegmentType segmentType = getSegmentType(static_cast<int>(i));
    sf::Sprite segment = [&]() -> sf::Sprite {
      switch (segmentType) {
        case utils::SnakeSprite::SegmentType::Head:
          return snakeSprite.getHeadSprite(getDirectionRotation());
        case utils::SnakeSprite::SegmentType::Body:
          return snakeSprite.getBodySprite(getBodySegmentRotation(static_cast<int>(i)));
        case utils::SnakeSprite::SegmentType::BodyCorner:
          return snakeSprite.getBodyCornerSprite(getBodyCornerRotation(static_cast<int>(i)));
        case utils::SnakeSprite::SegmentType::Tail:
          return snakeSprite.getTailSprite(getTailRotation());
        default:
          return snakeSprite.getBodySprite();
      }
    }();

    // Position the segment
    sf::Vector2f position = grid.getCellPosition(body[i].y, body[i].x);
    sf::Vector2f centerOffset(grid.getScaledCellSize() / 2.0f, grid.getScaledCellSize() / 2.0f);
    segment.setPosition(position + centerOffset);

    // Scale the sprite to fit the cell
    float scale = grid.getScaledCellSize() / 28.0f;  // 28 is the sprite size
    segment.setScale(sf::Vector2f(scale, scale));

    // Apply blinking effect with animated transparency
    if (blinking) {
      // Calculate animated alpha based on time
      float time = blinkTimer.getElapsedTime().asSeconds();
      float alpha = 128 + 127 * std::sin(time * 3.14159f * 4.0f);  // Oscillates between 128 and 255
      segment.setColor(sf::Color(255, 255, 255, static_cast<unsigned char>(alpha)));

    } else {
      segment.setColor(sf::Color::White);
    }

    window.draw(segment);
  }

  // Update tongue timer
  updateTongue(grid);

  // Render tongue based on timer (only if snake is alive)
  if (tongueVisible && isAlive()) {
    // Position the tongue in front of the head based on direction
    sf::Vector2f headPosition = grid.getCellPosition(body[0].y, body[0].x);
    sf::Vector2f centerOffset(grid.getScaledCellSize() / 2.0f, grid.getScaledCellSize() / 2.0f);

    // Add offset based on snake direction to position tongue in front of head
    sf::Vector2f tongueOffset(0.0f, 0.0f);
    switch (currentDirection) {
      case Direction::Up:
        tongueOffset.y = -grid.getScaledCellSize() * 1.5f;  // Above the head
        break;
      case Direction::Down:
        tongueOffset.y = grid.getScaledCellSize() * 1.5f;  // Below the head
        break;
      case Direction::Left:
        tongueOffset.x = -grid.getScaledCellSize() * 1.5f;  // Left of the head
        break;
      case Direction::Right:
        tongueOffset.x = grid.getScaledCellSize() * 1.5f;  // Right of the head
        break;
    }

    tongueSprite.setPosition(headPosition + centerOffset + tongueOffset);

    // Scale the sprite to fit the cell (make tongue bigger)
    float scale = grid.getScaledCellSize() / 28.0f;  // 28 is the sprite size
    float tongueScale = scale * 2.0f;                // Make tongue 50% bigger
    tongueSprite.setScale(sf::Vector2f(tongueScale, tongueScale));

    tongueSprite.setRotation(sf::degrees(getDirectionRotation()));

    window.draw(tongueSprite);
  }
}

void Snake::updateDirection() {
  currentDirection = nextDirection;
}

void Snake::updateTongue(const utils::GameGrid& grid) const {
  // Don't update tongue if snake is dead
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

      // Don't position here - will be positioned in render method

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

  // Calculate direction from current segment to previous segment
  sf::Vector2i current = body[segmentIndex];
  sf::Vector2i previous = body[segmentIndex - 1];

  sf::Vector2i direction = current - previous;

  // Determine rotation based on direction vector
  if (direction.x > 0) {
    return 90.0f;  // Moving right
  } else if (direction.x < 0) {
    return 270.0f;  // Moving left
  } else if (direction.y > 0) {
    return 180.0f;  // Moving down
  } else if (direction.y < 0) {
    return 0.0f;  // Moving up
  }

  return 0.0f;  // Default
}

float Snake::getBodyCornerRotation(int segmentIndex) const {
  if (segmentIndex <= 0 || segmentIndex >= static_cast<int>(body.size()) - 1) {
    return 0.0f;
  }

  // Get the three consecutive segments
  sf::Vector2i prev = body[segmentIndex - 1];
  sf::Vector2i current = body[segmentIndex];
  sf::Vector2i next = body[segmentIndex + 1];

  // Calculate direction vectors
  sf::Vector2i dirFromPrev = current - prev;
  sf::Vector2i dirToNext = next - current;

  // Determine rotation based on the specific turn combination
  // Clockwise turns
  // Up -> Right (0° -> 90°)
  if (dirFromPrev.y < 0 && dirToNext.x > 0)
    return 0.0f;
  // Right -> Down (90° -> 180°)
  if (dirFromPrev.x > 0 && dirToNext.y > 0)
    return 90.0f;
  // Down -> Left (180° -> 270°)
  if (dirFromPrev.y > 0 && dirToNext.x < 0)
    return 180.0f;
  // Left -> Up (270° -> 0°)
  if (dirFromPrev.x < 0 && dirToNext.y < 0)
    return 270.0f;

  // Counter-clockwise turns
  // Up -> Left (0° -> 270°)
  if (dirFromPrev.y < 0 && dirToNext.x < 0)
    return 90.0f;
  // Left -> Down (270° -> 180°)
  if (dirFromPrev.x < 0 && dirToNext.y > 0)
    return 0.0f;
  // Down -> Right (180° -> 90°)
  if (dirFromPrev.y > 0 && dirToNext.x > 0)
    return 270.0f;
  // Right -> Up (90° -> 0°)
  if (dirFromPrev.x > 0 && dirToNext.y < 0)
    return 180.0f;

  return 0.0f;  // Default
}

float Snake::getTailRotation() const {
  if (body.size() < 2) {
    return 0.0f;
  }

  // Calculate direction from tail to second-to-last segment
  sf::Vector2i tail = body.back();
  sf::Vector2i secondToLast = body[body.size() - 2];

  sf::Vector2i direction = tail - secondToLast;

  // Tail should point in the opposite direction of its movement
  if (direction.x > 0) {
    return 270.0f;  // Tail pointing left (opposite of moving right)
  } else if (direction.x < 0) {
    return 90.0f;  // Tail pointing right (opposite of moving left)
  } else if (direction.y > 0) {
    return 0.0f;  // Tail pointing up (opposite of moving down)
  } else if (direction.y < 0) {
    return 180.0f;  // Tail pointing down (opposite of moving up)
  }

  return 0.0f;  // Default
}

utils::SnakeSprite::SegmentType Snake::getSegmentType(int segmentIndex) const {
  if (segmentIndex == 0) {
    return utils::SnakeSprite::SegmentType::Head;
  } else if (segmentIndex == static_cast<int>(body.size()) - 1) {
    return utils::SnakeSprite::SegmentType::Tail;
  } else if (isBodyCorner(segmentIndex)) {
    return utils::SnakeSprite::SegmentType::BodyCorner;
  } else {
    return utils::SnakeSprite::SegmentType::Body;
  }
}

bool Snake::isBodyCorner(int segmentIndex) const {
  if (segmentIndex <= 0 || segmentIndex >= static_cast<int>(body.size()) - 1) {
    return false;  // Head and tail are never corners
  }

  // Get the three consecutive segments
  sf::Vector2i prev = body[segmentIndex - 1];
  sf::Vector2i current = body[segmentIndex];
  sf::Vector2i next = body[segmentIndex + 1];

  // Calculate direction vectors
  sf::Vector2i dirFromPrev = current - prev;
  sf::Vector2i dirToNext = next - current;

  // Check if directions are perpendicular (corner)
  // If both directions are not the same, it's a corner
  return (dirFromPrev.x != dirToNext.x) || (dirFromPrev.y != dirToNext.y);
}
