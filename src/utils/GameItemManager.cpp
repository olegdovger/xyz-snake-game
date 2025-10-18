#include "GameItemManager.hpp"
#include <algorithm>
#include "../Snake.hpp"
#include "FantomApple.hpp"
#include "GreenApple.hpp"
#include "RedApple.hpp"
#include "WaterBubble.hpp"

class GameGrid;

GameItemManager::GameItemManager(const GameGrid& grid, const DifficultySettings& difficultySettings)
    : grid(grid),
      difficultySettings(difficultySettings),
      randomGenerator(std::random_device{}()),
      itemTypeDistribution(0, 3),  // 0=RedApple, 1=GreenApple, 2=WaterBubble, 3=FantomApple
      positionDistributionX(0, grid.getCols() - 1),
      positionDistributionY(0, grid.getRows() - 1) {}

void GameItemManager::update(float deltaTime, const Snake& snake) {
  // Remove expired items
  removeExpiredItems();

  // Update all items
  for (auto& item : items) {
    item->update(deltaTime);
  }

  // Try to spawn new item
  if (spawnTimer.getElapsedTime().asSeconds() >= difficultySettings.getItemSpawnInterval() && 
      items.size() < difficultySettings.getMaxItemsOnBoard()) {
    spawnRandomItem(snake);
    spawnTimer.restart();
  }
}

void GameItemManager::render(sf::RenderWindow& window, const GameGrid& grid) const {
  for (const auto& item : items) {
    item->render(window, grid);
  }
}

GameItem* GameItemManager::checkCollision(sf::Vector2i snakeHead) {
  for (auto& item : items) {
    if (item->checkCollision(snakeHead)) {
      return item.get();
    }
  }
  return nullptr;
}

bool GameItemManager::spawnRandomItem(const Snake& snake) {
  // Use difficulty-based apple probabilities
  std::uniform_real_distribution<float> probabilityDistribution(0.0f, 1.0f);
  float randomValue = probabilityDistribution(randomGenerator);
  
  GameItemType type;
  float cumulativeChance = 0.0f;
  
  cumulativeChance += difficultySettings.getRedAppleChance();
  if (randomValue <= cumulativeChance) {
    type = GameItemType::RedApple;
  } else {
    cumulativeChance += difficultySettings.getGreenAppleChance();
    if (randomValue <= cumulativeChance) {
      type = GameItemType::GreenApple;
    } else {
      cumulativeChance += difficultySettings.getWaterBubbleChance();
      if (randomValue <= cumulativeChance) {
        type = GameItemType::WaterBubble;
      } else {
        type = GameItemType::FantomApple;
      }
    }
  }
  
  return spawnItem(type, snake);
}

bool GameItemManager::spawnItem(GameItemManager::GameItemType itemType, const Snake& snake) {
  if (items.size() >= difficultySettings.getMaxItemsOnBoard()) {
    return false;
  }

  sf::Vector2i position = generateRandomPosition(snake);
  if (!isValidPosition(position, snake)) {
    return false;
  }

  std::unique_ptr<GameItem> item;
  switch (itemType) {
    case GameItemType::RedApple:
      item = std::make_unique<RedApple>(position, grid.getCols(), grid.getRows(), snake.getSpeed(), difficultySettings.getAppleLifetimeMultiplier());
      break;
    case GameItemType::GreenApple:
      item = std::make_unique<GreenApple>(position, difficultySettings.getAppleLifetimeMultiplier());
      break;
    case GameItemType::WaterBubble:
      item = std::make_unique<WaterBubble>(position, difficultySettings.getAppleLifetimeMultiplier());
      break;
    case GameItemType::FantomApple:
      item = std::make_unique<FantomApple>(position, difficultySettings.getAppleLifetimeMultiplier());
      break;
  }

  if (item) {
    items.push_back(std::move(item));
    return true;
  }

  return false;
}

sf::Vector2i GameItemManager::generateRandomPosition(const Snake& snake) const {
  return sf::Vector2i(positionDistributionX(randomGenerator), positionDistributionY(randomGenerator));
}

bool GameItemManager::isValidPosition(sf::Vector2i position, const Snake& snake) const {
  // Check if position is within grid bounds
  if (!grid.isValidPosition(position.y, position.x)) {
    return false;
  }

  // Check if position conflicts with snake
  if (snake.checkCollisionWithPosition(position)) {
    return false;
  }

  // Check if position conflicts with existing items
  for (const auto& item : items) {
    if (item->checkCollision(position)) {
      return false;
    }
  }

  return true;
}

template <typename Predicate>
void GameItemManager::removeItemsIf(Predicate predicate) {
  items.erase(std::remove_if(items.begin(), items.end(), predicate), items.end());
}

void GameItemManager::removeItem(GameItem* item) {
  if (item) {
    removeItemsIf([item](const std::unique_ptr<GameItem>& gameItem) { return gameItem.get() == item; });
  }
}

void GameItemManager::removeExpiredItems() {
  removeItemsIf([](const std::unique_ptr<GameItem>& item) { return item->isExpired(); });
}

void GameItemManager::clear() {
  items.clear();
}
