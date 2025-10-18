#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>
#include <vector>
#include "PausableClock.hpp"
#include "Wall.hpp"
#include "difficulty/DifficultySettings.hpp"

class GameGrid;
class Snake;

class WallManager {
public:
  explicit WallManager(const GameGrid& grid, const DifficultySettings& difficulty);

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
  const DifficultySettings& difficultySettings;
  std::vector<std::unique_ptr<Wall>> walls;

  // Wall generation timer
  PausableClock wallGenerationTimer;
  static constexpr float WALL_GENERATION_INTERVAL = 10.0f;  // 20 seconds

  // Generation parameters
  static constexpr float MAX_COVERAGE_PERCENT = 5.0f;
  static constexpr int MIN_WALL_SIZE = 1;
  static constexpr int MAX_WALL_SIZE = 7;
  static constexpr int MIN_DISTANCE_BETWEEN_WALLS = 1;

  // Generation methods
  std::vector<sf::Vector2i> generateWallPositions(const Snake& snake) const;
  std::vector<sf::Vector2i> generateRandomWallShape(sf::Vector2i startPos, const Snake& snake) const;
  bool isValidWallPosition(const std::vector<sf::Vector2i>& positions, const Snake& snake) const;
  bool isPositionBehindSnake(sf::Vector2i position, const Snake& snake) const;
  bool isPositionInSnakeDirection(sf::Vector2i position, sf::Vector2i snakeHead, int direction) const;
  bool isPositionFarFromWalls(sf::Vector2i position) const;
  Wall::WallType getRandomWallType() const;

  // Utility methods
  int calculateTotalWallCells() const;
  void removeExpiredWalls();
};
