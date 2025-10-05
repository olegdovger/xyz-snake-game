#include "WallManager.hpp"
#include <algorithm>
#include <cmath>
#include <random>
#include "../Snake.hpp"
#include "GameGrid.hpp"

WallManager::WallManager(const GameGrid& grid) : grid(grid) {}

void WallManager::update(float deltaTime, const Snake& snake) {
  // Remove expired walls
  removeExpiredWalls();

  // Update all walls
  for (auto& wall : walls) {
    wall->update(deltaTime);
  }

  // Generate new wall every 20 seconds
  if (wallGenerationTimer.getElapsedTime().asSeconds() >= WALL_GENERATION_INTERVAL) {
    tryGenerateWall(snake);
    wallGenerationTimer.restart();
  }
}

void WallManager::render(sf::RenderWindow& window, const GameGrid& grid) const {
  for (const auto& wall : walls) {
    wall->render(window, grid);
  }
}

bool WallManager::tryGenerateWall(const Snake& snake) {
  // Check if we can add more walls (max 15% coverage)
  if (getWallCoveragePercent() >= MAX_COVERAGE_PERCENT) {
    return false;
  }

  // Generate wall positions
  auto positions = generateWallPositions(snake);
  if (positions.empty()) {
    return false;
  }

  // Validate the wall position
  if (!isValidWallPosition(positions, snake)) {
    return false;
  }

  // Create and add the wall
  auto wallType = getRandomWallType();
  walls.push_back(std::make_unique<Wall>(positions, wallType));

  return true;
}

bool WallManager::checkWallCollision(sf::Vector2i position) const {
  for (const auto& wall : walls) {
    if (wall->checkCollisionWithPosition(position) && wall->canCollide()) {
      return true;
    }
  }
  return false;
}

float WallManager::getWallCoveragePercent() const {
  int totalCells = grid.getRows() * grid.getCols();
  int wallCells = calculateTotalWallCells();
  return (static_cast<float>(wallCells) / static_cast<float>(totalCells)) * 100.0f;
}

std::vector<sf::Vector2i> WallManager::generateWallPositions(const Snake& snake) const {
  static std::random_device rd;
  static std::mt19937 gen(rd());

  // Get snake head and direction
  sf::Vector2i snakeHead = snake.getHead();
  Snake::Direction snakeDirection = snake.getDirection();

  // Determine preferred area (avoid snake's direction)
  std::vector<sf::Vector2i> candidatePositions;

  // Generate positions avoiding snake's direction
  for (int row = 0; row < grid.getRows(); ++row) {
    for (int col = 0; col < grid.getCols(); ++col) {
      sf::Vector2i position(row, col);  // row=x, col=y to match snake coordinate system

      // Skip if position is occupied by snake
      if (snake.checkCollisionWithPosition(position)) {
        continue;
      }

      // Skip positions in snake's direction
      int directionInt = static_cast<int>(snakeDirection);
      if (isPositionInSnakeDirection(position, snakeHead, directionInt)) {
        continue;
      }

      candidatePositions.push_back(position);
    }
  }

  if (candidatePositions.empty()) {
    return {};
  }

  // Randomly select a starting position
  std::uniform_int_distribution<int> posDis(0, static_cast<int>(candidatePositions.size()) - 1);
  sf::Vector2i startPos = candidatePositions[posDis(gen)];

  // Generate random wall shape
  return generateRandomWallShape(startPos, snake);
}

bool WallManager::isValidWallPosition(const std::vector<sf::Vector2i>& positions, const Snake& snake) const {
  for (const auto& position : positions) {
    // Check if position is valid on grid
    if (!grid.isValidPosition(position.y, position.x)) {
      return false;
    }

    // Check if position conflicts with snake
    if (snake.checkCollisionWithPosition(position)) {
      return false;
    }

    // Check if position is too close to existing walls
    if (!isPositionFarFromWalls(position)) {
      return false;
    }
  }

  return true;
}

bool WallManager::isPositionBehindSnake(sf::Vector2i position, const Snake& snake) const {
  sf::Vector2i snakeHead = snake.getHead();
  sf::Vector2i snakeTail = snake.getTail();

  // Calculate direction from tail to head
  sf::Vector2i snakeDirection = snakeHead - snakeTail;

  // Calculate direction from snake head to position
  sf::Vector2i toPosition = position - snakeHead;

  // Check if position is behind snake (dot product < 0)
  int dotProduct = snakeDirection.x * toPosition.x + snakeDirection.y * toPosition.y;
  return dotProduct < 0;
}

bool WallManager::isPositionInSnakeDirection(sf::Vector2i position, sf::Vector2i snakeHead, int direction) const {
  sf::Vector2i toPosition = position - snakeHead;

  // Check if position is in the snake's movement direction
  // Direction values: 0=Up, 1=Down, 2=Left, 3=Right
  switch (direction) {
    case 0:                     // Up
      return toPosition.y < 0;  // Position is above snake head
    case 1:                     // Down
      return toPosition.y > 0;  // Position is below snake head
    case 2:                     // Left
      return toPosition.x < 0;  // Position is left of snake head
    case 3:                     // Right
      return toPosition.x > 0;  // Position is right of snake head
  }
  return false;
}

std::vector<sf::Vector2i> WallManager::generateRandomWallShape(sf::Vector2i startPos, const Snake& snake) const {
  static std::random_device rd;
  static std::mt19937 gen(rd());

  std::vector<sf::Vector2i> wallPositions;
  wallPositions.push_back(startPos);

  // Generate wall size (1-7 cells)
  std::uniform_int_distribution<int> sizeDis(MIN_WALL_SIZE, MAX_WALL_SIZE);
  int wallSize = sizeDis(gen);

  // Generate random shape patterns
  std::uniform_int_distribution<int> patternDis(0, 4);  // 5 different patterns
  int pattern = patternDis(gen);

  sf::Vector2i currentPos = startPos;

  for (int i = 1; i < wallSize; ++i) {
    sf::Vector2i nextPos = currentPos;

    // Different wall patterns
    switch (pattern) {
      case 0:  // L-shape
        if (i == 1) {
          nextPos.x += (i % 2 == 0) ? 1 : -1;
        } else {
          nextPos.y += (i % 2 == 0) ? 1 : -1;
        }
        break;
      case 1:  // T-shape
        if (i == 1) {
          nextPos.x += 1;
        } else if (i == 2) {
          nextPos.x -= 1;
        } else {
          nextPos.y += 1;
        }
        break;
      case 2:  // Cross shape
        if (i == 1) {
          nextPos.x += 1;
        } else if (i == 2) {
          nextPos.x -= 1;
        } else if (i == 3) {
          nextPos.y += 1;
        } else {
          nextPos.y -= 1;
        }
        break;
      case 3:  // Random walk
      {
        std::uniform_int_distribution<int> dirDis(0, 3);
        int direction = dirDis(gen);
        switch (direction) {
          case 0:
            nextPos.y--;
            break;  // Up
          case 1:
            nextPos.y++;
            break;  // Down
          case 2:
            nextPos.x--;
            break;  // Left
          case 3:
            nextPos.x++;
            break;  // Right
        }
      } break;
      case 4:  // Square pattern
        if (i <= 2) {
          nextPos.x += 1;
        } else if (i == 3) {
          nextPos.y += 1;
        } else {
          nextPos.x -= 1;
        }
        break;
    }

    // Check if next position is valid
    if (!grid.isValidPosition(nextPos.y, nextPos.x)) {
      break;
    }

    // Check if next position conflicts with snake
    if (snake.checkCollisionWithPosition(nextPos)) {
      break;
    }

    // Check if position is too close to existing walls
    if (!isPositionFarFromWalls(nextPos)) {
      break;
    }

    wallPositions.push_back(nextPos);
    currentPos = nextPos;
  }

  return wallPositions;
}

bool WallManager::isPositionFarFromWalls(sf::Vector2i position) const {
  for (const auto& wall : walls) {
    for (const auto& wallPos : wall->getPositions()) {
      int distance = std::abs(position.x - wallPos.x) + std::abs(position.y - wallPos.y);
      if (distance <= MIN_DISTANCE_BETWEEN_WALLS) {
        return false;
      }
    }
  }
  return true;
}

Wall::WallType WallManager::getRandomWallType() const {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<int> dis(0, 3);

  return static_cast<Wall::WallType>(dis(gen));
}

int WallManager::calculateTotalWallCells() const {
  int total = 0;
  for (const auto& wall : walls) {
    total += static_cast<int>(wall->getPositions().size());
  }
  return total;
}

void WallManager::removeExpiredWalls() {
  walls.erase(
      std::remove_if(walls.begin(), walls.end(), [](const std::unique_ptr<Wall>& wall) { return wall->isExpired(); }),
      walls.end());
}
