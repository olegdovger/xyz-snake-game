#include "WallManager.hpp"
#include <algorithm>
#include <cmath>
#include <random>
#include "Snake.hpp"
#include "utils/GameGrid.hpp"

WallManager::WallManager(const GameGrid& grid) : grid(grid) {}

void WallManager::update(float deltaTime, const Snake& snake) {
  // Remove expired walls
  removeExpiredWalls();

  // Update all walls
  for (auto& wall : walls) {
    wall->update(deltaTime);
  }

  // Try to generate new wall occasionally
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<float> dis(0.0f, 1.0f);

  // 5% chance per update to try generating a wall
  if (dis(gen) < 0.05f) {
    tryGenerateWall(snake);
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
    if (wall->checkCollisionWithPosition(position)) {
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

  // Get snake body positions
  const auto& snakeBody = snake.getBody();
  sf::Vector2i snakeHead = snake.getHead();

  // Determine preferred area (behind snake)
  std::vector<sf::Vector2i> candidatePositions;

  // Generate positions behind the snake
  for (int row = 0; row < grid.getRows(); ++row) {
    for (int col = 0; col < grid.getCols(); ++col) {
      sf::Vector2i position(col, row);

      // Skip if position is occupied by snake
      if (snake.checkCollisionWithPosition(position)) {
        continue;
      }

      // Prefer positions behind snake
      if (isPositionBehindSnake(position, snake)) {
        candidatePositions.push_back(position);
      }
    }
  }

  if (candidatePositions.empty()) {
    return {};
  }

  // Randomly select a starting position
  std::uniform_int_distribution<int> posDis(0, static_cast<int>(candidatePositions.size()) - 1);
  sf::Vector2i startPos = candidatePositions[posDis(gen)];

  // Generate wall size (1-7 cells)
  std::uniform_int_distribution<int> sizeDis(MIN_WALL_SIZE, MAX_WALL_SIZE);
  int wallSize = sizeDis(gen);

  // Generate wall shape (simple line for now)
  std::vector<sf::Vector2i> wallPositions;
  wallPositions.push_back(startPos);

  // Try to extend the wall in a random direction
  std::uniform_int_distribution<int> dirDis(0, 3);  // 0=up, 1=down, 2=left, 3=right
  int direction = dirDis(gen);

  sf::Vector2i currentPos = startPos;
  for (int i = 1; i < wallSize; ++i) {
    sf::Vector2i nextPos = currentPos;

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

    // Check if next position is valid
    if (!grid.isValidPosition(nextPos.y, nextPos.x)) {
      break;
    }

    // Check if next position conflicts with snake
    if (snake.checkCollisionWithPosition(nextPos)) {
      break;
    }

    wallPositions.push_back(nextPos);
    currentPos = nextPos;
  }

  return wallPositions;
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
