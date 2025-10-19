#pragma once
#include <SFML/System/Time.hpp>

#include <cstdint>

class DifficultySettings {
public:
  DifficultySettings() = default;

  DifficultySettings(float baseSnakeSpeed, float speedIncreaseRate, float itemSpawnInterval, uint8_t maxItemsOnBoard,
                     float appleLifetimeMultiplier, float specialItemChance, uint8_t wallCount);

  float getBaseSnakeSpeed() const { return baseSnakeSpeed; }
  float getSpeedIncreaseRate() const { return speedIncreaseRate; }
  float getSpeedIncreaseInterval() const { return speedIncreaseInterval; }

  float getItemSpawnInterval() const { return itemSpawnInterval; }
  uint8_t getMaxItemsOnBoard() const { return maxItemsOnBoard; }
  float getAppleLifetimeMultiplier() const { return appleLifetimeMultiplier; }
  float getSpecialItemChance() const { return specialItemChance; }

  float getRedAppleChance() const { return redAppleChance; }
  float getGreenAppleChance() const { return greenAppleChance; }
  float getWaterBubbleChance() const { return waterBubbleChance; }
  float getFantomAppleChance() const { return fantomAppleChance; }

  uint8_t getWallCount() const { return wallCount; }
  float getWallDensity() const { return wallDensity; }

  float getScoreMultiplier() const { return scoreMultiplier; }
  float getEffectDurationMultiplier() const { return effectDurationMultiplier; }

  void setSnakeParameters(float baseSpeed, float increaseRate, float increaseInterval);

  void setItemParameters(float spawnInterval, uint8_t maxItems, float lifetimeMultiplier, float specialChance);

  void setAppleChances(float red, float green, float water, float fantom);

  void setWallParameters(uint8_t count, float density);

  void setMultipliers(float score, float effectDuration);

  bool validate() const;

private:
  float baseSnakeSpeed = 5.0f;
  float speedIncreaseRate = 0.5f;
  float speedIncreaseInterval = 5.0f;

  float itemSpawnInterval = 3.0f;
  uint8_t maxItemsOnBoard = 5;
  float appleLifetimeMultiplier = 1.0f;
  float specialItemChance = 0.3f;

  float redAppleChance = 0.5f;
  float greenAppleChance = 0.2f;
  float waterBubbleChance = 0.2f;
  float fantomAppleChance = 0.1f;

  uint8_t wallCount = 0;
  float wallDensity = 0.1f;

  float scoreMultiplier = 1.0f;
  float effectDurationMultiplier = 1.0f;
};