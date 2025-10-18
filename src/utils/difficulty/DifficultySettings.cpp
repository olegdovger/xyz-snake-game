#include "DifficultySettings.hpp"
#include <cmath>
#include <stdexcept>
#include <iostream>

DifficultySettings::DifficultySettings(
    float baseSnakeSpeed,
    float speedIncreaseRate,
    float itemSpawnInterval,
    uint8_t maxItemsOnBoard,
    float appleLifetimeMultiplier,
    float specialItemChance,
    uint8_t wallCount
) : baseSnakeSpeed(baseSnakeSpeed),
    speedIncreaseRate(speedIncreaseRate),
    itemSpawnInterval(itemSpawnInterval),
    maxItemsOnBoard(maxItemsOnBoard),
    appleLifetimeMultiplier(appleLifetimeMultiplier),
    specialItemChance(specialItemChance),
    wallCount(wallCount) {
    
    // Set default apple chances
    setAppleChances(0.5f, 0.2f, 0.2f, 0.1f);
}

void DifficultySettings::setSnakeParameters(float baseSpeed, float increaseRate, float increaseInterval) {
    if (baseSpeed <= 0.0f || increaseRate < 0.0f || increaseInterval <= 0.0f) {
        throw std::invalid_argument("Snake parameters must be positive");
    }
    
    baseSnakeSpeed = baseSpeed;
    speedIncreaseRate = increaseRate;
    speedIncreaseInterval = increaseInterval;
}

void DifficultySettings::setItemParameters(float spawnInterval, uint8_t maxItems, float lifetimeMultiplier, float specialChance) {
    if (spawnInterval <= 0.0f || maxItems == 0 || lifetimeMultiplier <= 0.0f) {
        throw std::invalid_argument("Item parameters must be positive");
    }
    
    if (specialChance < 0.0f || specialChance > 1.0f) {
        throw std::invalid_argument("Special item chance must be between 0.0 and 1.0");
    }
    
    itemSpawnInterval = spawnInterval;
    maxItemsOnBoard = maxItems;
    appleLifetimeMultiplier = lifetimeMultiplier;
    specialItemChance = specialChance;
}

void DifficultySettings::setAppleChances(float red, float green, float water, float fantom) {
    const float total = red + green + water + fantom;
    
    if (total <= 0.0f || std::abs(total - 1.0f) > 0.001f) {
        throw std::invalid_argument("Apple chances must sum to 1.0");
    }
    
    if (red < 0.0f || green < 0.0f || water < 0.0f || fantom < 0.0f) {
        throw std::invalid_argument("Apple chances must be non-negative");
    }
    
    redAppleChance = red;
    greenAppleChance = green;
    waterBubbleChance = water;
    fantomAppleChance = fantom;
}

void DifficultySettings::setWallParameters(uint8_t count, float density) {
    if (density < 0.0f || density > 1.0f) {
        throw std::invalid_argument("Wall density must be between 0.0 and 1.0");
    }
    
    wallCount = count;
    wallDensity = density;
}

void DifficultySettings::setMultipliers(float score, float effectDuration) {
    if (score <= 0.0f || effectDuration <= 0.0f) {
        throw std::invalid_argument("Multipliers must be positive");
    }
    
    scoreMultiplier = score;
    effectDurationMultiplier = effectDuration;
}

bool DifficultySettings::validate() const {
    // Check snake parameters
    if (baseSnakeSpeed <= 0.0f || speedIncreaseRate < 0.0f || speedIncreaseInterval <= 0.0f) {
        std::cerr << "Invalid snake parameters" << std::endl;
        return false;
    }
    
    // Check item parameters
    if (itemSpawnInterval <= 0.0f || maxItemsOnBoard == 0 || appleLifetimeMultiplier <= 0.0f) {
        std::cerr << "Invalid item parameters" << std::endl;
        return false;
    }
    
    if (specialItemChance < 0.0f || specialItemChance > 1.0f) {
        std::cerr << "Invalid special item chance" << std::endl;
        return false;
    }
    
    // Check apple chances
    const float totalChance = redAppleChance + greenAppleChance + waterBubbleChance + fantomAppleChance;
    if (std::abs(totalChance - 1.0f) > 0.001f) {
        std::cerr << "Apple chances must sum to 1.0, got: " << totalChance << std::endl;
        return false;
    }
    
    // Check wall parameters
    if (wallDensity < 0.0f || wallDensity > 1.0f) {
        std::cerr << "Invalid wall density" << std::endl;
        return false;
    }
    
    // Check multipliers
    if (scoreMultiplier <= 0.0f || effectDurationMultiplier <= 0.0f) {
        std::cerr << "Invalid multipliers" << std::endl;
        return false;
    }
    
    return true;
}