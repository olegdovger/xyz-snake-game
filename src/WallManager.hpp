#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Wall.hpp"

class GameGrid;

class Snake;

class WallManager {
public:
  explicit WallManager(const GameGrid& grid);

  // Wall management
  void update(float deltaTime, const Snake& snake);
  void render(sf::RenderWindow& window, const GameGrid& grid) const;

  // Wall generation
  bool tryGenerateWall(const Snake& snake);

  // Collision detection
  bool checkWallCollision(sf::Vector2i position) const;

  // Statistics
  int getWallCount() const { return static_cast<int>(walls.size()); }
  float getWallCoveragePercent() const;

private:
  const GameGrid& grid;
  std::vector<std::unique_ptr<Wall>> walls;

  // Generation parameters
  static constexpr float MAX_COVERAGE_PERCENT = 15.0f;
  static constexpr int MIN_WALL_SIZE = 1;
  static constexpr int MAX_WALL_SIZE = 7;
  static constexpr int MIN_DISTANCE_BETWEEN_WALLS = 1;

  // Generation methods
  std::vector<sf::Vector2i> generateWallPositions(const Snake& snake) const;
  bool isValidWallPosition(const std::vector<sf::Vector2i>& positions, const Snake& snake) const;
  bool isPositionBehindSnake(sf::Vector2i position, const Snake& snake) const;
  bool isPositionFarFromWalls(sf::Vector2i position) const;
  Wall::WallType getRandomWallType() const;

  // Utility methods
  int calculateTotalWallCells() const;
  void removeExpiredWalls();
};
