#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <random>
#include <vector>
#include "GameGrid.hpp"
#include "GameItem.hpp"
#include "difficulty/DifficultySettings.hpp"

class GameGrid;
class GameItem;
class Snake;

class GameItemManager {
public:
  explicit GameItemManager(const GameGrid& grid, const DifficultySettings& difficultySettings);

  void update(float deltaTime, const Snake& snake);

  void render(sf::RenderWindow& window, const GameGrid& grid) const;

  GameItem* checkCollision(sf::Vector2i snakeHead);

  enum class GameItemType { RedApple, GreenApple, WaterBubble, FantomApple };

  bool spawnRandomItem(const Snake& snake);

  bool spawnItem(GameItemType itemType, const Snake& snake);

  int getItemCount() const { return static_cast<int>(items.size()); }

  void removeItem(GameItem* item);

  void clear();

private:
  const GameGrid& grid;
  std::vector<std::unique_ptr<GameItem>> items;
  mutable std::mt19937 randomGenerator;
  mutable std::uniform_int_distribution<int> itemTypeDistribution;
  mutable std::uniform_int_distribution<int> positionDistributionX;
  mutable std::uniform_int_distribution<int> positionDistributionY;

  const DifficultySettings& difficultySettings;
  sf::Clock spawnTimer;

  sf::Vector2i generateRandomPosition(const Snake& snake) const;

  bool isValidPosition(sf::Vector2i position, const Snake& snake) const;

  void removeExpiredItems();

  template <typename Predicate>
  void removeItemsIf(Predicate predicate);
};
