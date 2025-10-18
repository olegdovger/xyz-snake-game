#pragma once
#include <SFML/System/Time.hpp>

#include <cstdint>

/**
 * @brief Structure containing all difficulty-related game parameters
 * 
 * This class encapsulates all the parameters that define game difficulty,
 * including snake speed, item spawning rates, and various gameplay modifiers.
 */
class DifficultySettings {
public:
    /**
     * @brief Default constructor with easy difficulty settings
     */
    DifficultySettings() = default;

    /**
     * @brief Constructor with specific difficulty parameters
     */
    DifficultySettings(
        float baseSnakeSpeed,
        float speedIncreaseRate,
        float itemSpawnInterval,
        uint8_t maxItemsOnBoard,
        float appleLifetimeMultiplier,
        float specialItemChance,
        uint8_t wallCount
    );

    // Snake parameters
    float getBaseSnakeSpeed() const { return baseSnakeSpeed; }
    float getSpeedIncreaseRate() const { return speedIncreaseRate; }
    float getSpeedIncreaseInterval() const { return speedIncreaseInterval; }

    // Item spawning parameters
    float getItemSpawnInterval() const { return itemSpawnInterval; }
    uint8_t getMaxItemsOnBoard() const { return maxItemsOnBoard; }
    float getAppleLifetimeMultiplier() const { return appleLifetimeMultiplier; }
    float getSpecialItemChance() const { return specialItemChance; }

    // Apple type spawn chances (0.0 to 1.0, should sum to 1.0)
    float getRedAppleChance() const { return redAppleChance; }
    float getGreenAppleChance() const { return greenAppleChance; }
    float getWaterBubbleChance() const { return waterBubbleChance; }
    float getFantomAppleChance() const { return fantomAppleChance; }

    // Wall parameters
    uint8_t getWallCount() const { return wallCount; }
    float getWallDensity() const { return wallDensity; }

    // Score multipliers
    float getScoreMultiplier() const { return scoreMultiplier; }
    
    // Effect duration multipliers
    float getEffectDurationMultiplier() const { return effectDurationMultiplier; }

    /**
     * @brief Set snake speed parameters
     */
    void setSnakeParameters(float baseSpeed, float increaseRate, float increaseInterval);

    /**
     * @brief Set item spawning parameters
     */
    void setItemParameters(float spawnInterval, uint8_t maxItems, float lifetimeMultiplier, float specialChance);

    /**
     * @brief Set apple spawn chances (values should sum to 1.0)
     */
    void setAppleChances(float red, float green, float water, float fantom);

    /**
     * @brief Set wall parameters
     */
    void setWallParameters(uint8_t count, float density);

    /**
     * @brief Set score and effect multipliers
     */
    void setMultipliers(float score, float effectDuration);

    /**
     * @brief Validate settings (ensure probabilities sum to 1.0, etc.)
     */
    bool validate() const;

private:
    // Snake parameters
    float baseSnakeSpeed = 5.0f;              // Base movement speed
    float speedIncreaseRate = 0.5f;           // Speed increase amount
    float speedIncreaseInterval = 5.0f;       // Seconds between speed increases

    // Item spawning parameters
    float itemSpawnInterval = 3.0f;           // Seconds between item spawns
    uint8_t maxItemsOnBoard = 5;              // Maximum items simultaneously
    float appleLifetimeMultiplier = 1.0f;     // Multiplier for apple lifetime
    float specialItemChance = 0.3f;           // Chance for special items (non-red apples)

    // Apple type spawn chances (should sum to 1.0)
    float redAppleChance = 0.5f;              // 50% chance
    float greenAppleChance = 0.2f;            // 20% chance
    float waterBubbleChance = 0.2f;           // 20% chance
    float fantomAppleChance = 0.1f;           // 10% chance

    // Wall parameters
    uint8_t wallCount = 0;                    // Number of wall segments
    float wallDensity = 0.1f;                 // Wall density (0.0 to 1.0)

    // Multipliers
    float scoreMultiplier = 1.0f;             // Score multiplier
    float effectDurationMultiplier = 1.0f;    // Effect duration multiplier
};