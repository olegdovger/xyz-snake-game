#include "WallManager.hpp"
#include <algorithm>
#include <cmath>
#include <random>
#include "../Snake.hpp"
#include "GameGrid.hpp"

WallManager::WallManager(const GameGrid& grid, const DifficultySettings& difficulty)
    : grid(grid), difficultySettings(difficulty) {}

void WallManager::update(float deltaTime, const Snake& snake) {
  removeExpiredWalls();

  for (auto& wall : walls) {
    wall->update(deltaTime);
  }

  float baseInterval = 20.0f;
  float difficultyMultiplier = 1.0f + (difficultySettings.getWallCount() * 0.5f);
  float adjustedInterval = baseInterval / difficultyMultiplier;

  if (wallGenerationTimer.getElapsedTime() >= adjustedInterval) {
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
  if (getWallCount() >= difficultySettings.getWallCount()) {
    return false;
  }

  float maxCoverage = 5.0f + (difficultySettings.getWallCount() * 1.5f);

  if (getWallCoveragePercent() >= maxCoverage) {
    return false;
  }

  auto positions = generateWallPositions(snake);
  if (positions.empty()) {
    return false;
  }

  if (!isValidWallPosition(positions, snake)) {
    return false;
  }

  auto wallType = getRandomWallType();
  walls.push_back(std::make_unique<Wall>(positions, wallType, &difficultySettings));

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

  sf::Vector2i snakeHead = snake.getHead();
  Snake::Direction snakeDirection = snake.getDirection();

  std::vector<sf::Vector2i> candidatePositions;

  for (int row = 0; row < grid.getRows(); ++row) {
    for (int col = 0; col < grid.getCols(); ++col) {
      sf::Vector2i position(row, col);

      if (snake.checkCollisionWithPosition(position)) {
        continue;
      }

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

  std::uniform_int_distribution<int> posDis(0, static_cast<int>(candidatePositions.size()) - 1);
  sf::Vector2i startPos = candidatePositions[posDis(gen)];

  return generateRandomWallShape(startPos, snake);
}

bool WallManager::isValidWallPosition(const std::vector<sf::Vector2i>& positions, const Snake& snake) const {
  for (const auto& position : positions) {
    if (!grid.isValidPosition(position.y, position.x)) {
      return false;
    }

    if (snake.checkCollisionWithPosition(position)) {
      return false;
    }

    if (!isPositionFarFromWalls(position)) {
      return false;
    }
  }

  return true;
}

bool WallManager::isPositionBehindSnake(sf::Vector2i position, const Snake& snake) const {
  sf::Vector2i snakeHead = snake.getHead();
  sf::Vector2i snakeTail = snake.getTail();

  sf::Vector2i snakeDirection = snakeHead - snakeTail;

  sf::Vector2i toPosition = position - snakeHead;

  int dotProduct = snakeDirection.x * toPosition.x + snakeDirection.y * toPosition.y;
  return dotProduct < 0;
}

bool WallManager::isPositionInSnakeDirection(sf::Vector2i position, sf::Vector2i snakeHead, int direction) const {
  sf::Vector2i toPosition = position - snakeHead;

  switch (direction) {
    case 0:
      return toPosition.y < 0;
    case 1:
      return toPosition.y > 0;
    case 2:
      return toPosition.x < 0;
    case 3:
      return toPosition.x > 0;
  }
  return false;
}

std::vector<sf::Vector2i> WallManager::generateRandomWallShape(sf::Vector2i startPos, const Snake& snake) const {
  static std::random_device rd;
  static std::mt19937 gen(rd());

  std::vector<sf::Vector2i> wallPositions;
  wallPositions.push_back(startPos);

  int minWallSize =
      std::min(MAX_WALL_SIZE - 1, MIN_WALL_SIZE + static_cast<int>(difficultySettings.getWallCount() / 2));
  int maxWallSize = std::max(minWallSize + 1, MAX_WALL_SIZE - static_cast<int>(difficultySettings.getWallCount() / 3));
  std::uniform_int_distribution<int> sizeDis(minWallSize, maxWallSize);
  int wallSize = sizeDis(gen);

  std::uniform_int_distribution<int> patternDis(0, 4);  // 5 different patterns
  int pattern = patternDis(gen);

  sf::Vector2i currentPos = startPos;

  for (int i = 1; i < wallSize; ++i) {
    sf::Vector2i nextPos = currentPos;

    switch (pattern) {
      case 0:
        if (i == 1) {
          nextPos.x += (i % 2 == 0) ? 1 : -1;
        } else {
          nextPos.y += (i % 2 == 0) ? 1 : -1;
        }
        break;
      case 1:
        if (i == 1) {
          nextPos.x += 1;
        } else if (i == 2) {
          nextPos.x -= 1;
        } else {
          nextPos.y += 1;
        }
        break;
      case 2:
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
      case 3: {
        std::uniform_int_distribution<int> dirDis(0, 3);
        int direction = dirDis(gen);
        switch (direction) {
          case 0:
            nextPos.y--;
            break;
          case 1:
            nextPos.y++;
            break;
          case 2:
            nextPos.x--;
            break;
          case 3:
            nextPos.x++;
            break;
        }
      } break;
      case 4:
        if (i <= 2) {
          nextPos.x += 1;
        } else if (i == 3) {
          nextPos.y += 1;
        } else {
          nextPos.x -= 1;
        }
        break;
    }

    if (!grid.isValidPosition(nextPos.y, nextPos.x)) {
      break;
    }

    if (snake.checkCollisionWithPosition(nextPos)) {
      break;
    }

    if (!isPositionFarFromWalls(nextPos)) {
      break;
    }

    wallPositions.push_back(nextPos);
    currentPos = nextPos;
  }

  return wallPositions;
}

bool WallManager::isPositionFarFromWalls(sf::Vector2i position) const {
  int minDistance = std::max(1, 3 - static_cast<int>(difficultySettings.getWallCount() / 2));

  for (const auto& wall : walls) {
    for (const auto& wallPos : wall->getPositions()) {
      int distance = std::abs(position.x - wallPos.x) + std::abs(position.y - wallPos.y);
      if (distance <= minDistance) {
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