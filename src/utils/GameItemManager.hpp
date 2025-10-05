#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <vector>
#include "GameGrid.hpp"
#include "GameItem.hpp"

class GameGrid;
class GameItem;

// Forward declarations
class Snake;

/**
 * @brief Manages all game items in the game
 * 
 * Handles spawning, updating, and collision detection for all game item types.
 */
class GameItemManager {
public:
  /**
     * @brief Constructor
     * @param grid Game grid reference
     */
  explicit GameItemManager(const GameGrid& grid);

  /**
     * @brief Update all game items
     * @param deltaTime Time elapsed since last update
     * @param snake Snake reference for collision detection
     */
  void update(float deltaTime, const Snake& snake);

  /**
     * @brief Render all game items
     * @param window Render window
     * @param grid Game grid for position conversion
     */
  void render(sf::RenderWindow& window, const GameGrid& grid) const;

  /**
     * @brief Check for game item collisions with snake head
     * @param snakeHead Snake head position
     * @return Pointer to collided item, nullptr if none
     */
  GameItem* checkCollision(sf::Vector2i snakeHead);

  enum class GameItemType { RedApple, GreenApple, WaterBubble, FantomApple };

  /**
     * @brief Spawn a random game item
     * @param snake Snake reference for position validation
     * @return true if item was spawned successfully
     */
  bool spawnRandomItem(const Snake& snake);

  /**
     * @brief Spawn a specific game item type
     * @param itemType Type of item to spawn
     * @param snake Snake reference for position validation
     * @return true if item was spawned successfully
     */
  bool spawnItem(GameItemType itemType, const Snake& snake);

  /**
     * @brief Get current game item count
     * @return Number of active items
     */
  int getItemCount() const { return static_cast<int>(items.size()); }

  /**
     * @brief Clear all game items
     */
  void clear();

private:
  const GameGrid& grid;
  std::vector<std::unique_ptr<GameItem>> items;
  mutable std::mt19937 randomGenerator;
  mutable std::uniform_int_distribution<int> itemTypeDistribution;
  mutable std::uniform_int_distribution<int> positionDistributionX;
  mutable std::uniform_int_distribution<int> positionDistributionY;

  // Spawning parameters
  static constexpr float SPAWN_INTERVAL = 3.0f;  // seconds
  static constexpr int MAX_ITEMS = 5;
  sf::Clock spawnTimer;

  /**
     * @brief Generate random position for game item
     * @param snake Snake reference for collision avoidance
     * @return Random valid position
     */
  sf::Vector2i generateRandomPosition(const Snake& snake) const;

  /**
     * @brief Check if position is valid for game item spawning
     * @param position Position to check
     * @param snake Snake reference for collision check
     * @return true if position is valid
     */
  bool isValidPosition(sf::Vector2i position, const Snake& snake) const;

  /**
     * @brief Remove expired game items
     */
  void removeExpiredItems();
};
