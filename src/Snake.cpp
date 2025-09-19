#include "Snake.hpp"
#include <algorithm>
#include "SnakeSprite.hpp"
#include "utils/GameGrid.hpp"

Snake::Snake(sf::Vector2i startPosition, int initialLength)
    : currentDirection(Direction::Right),
      nextDirection(Direction::Right),
      alive(true),
      directionChanged(false),
      growthEnabled(false),
      snakeSprite(utils::SnakeSprite::SnakeType::Green) {
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
  if (body.size() < 4)
    return false;  // Need at least 4 segments to collide with self

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
  if (!alive)
    return;

  for (size_t i = 0; i < body.size(); ++i) {
    // Get sprite for this segment
    utils::SnakeSprite::SegmentType segmentType = getSegmentType(static_cast<int>(i));
    sf::Sprite segment = [&]() -> sf::Sprite {
      switch (segmentType) {
        case utils::SnakeSprite::SegmentType::Head:
          return snakeSprite.getHeadSprite(getDirectionRotation());
        case utils::SnakeSprite::SegmentType::Body:
          return snakeSprite.getBodySprite(getBodySegmentRotation(static_cast<int>(i)));
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

    window.draw(segment);
  }
}

void Snake::updateDirection() {
  currentDirection = nextDirection;
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
  } else {
    return utils::SnakeSprite::SegmentType::Body;
  }
}
