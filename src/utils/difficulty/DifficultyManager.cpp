#include "DifficultyManager.hpp"
#include <stdexcept>

// Static member definitions
DifficultySettings DifficultyManager::easySettings;
DifficultySettings DifficultyManager::harderThanEasySettings;
DifficultySettings DifficultyManager::middleSettings;
DifficultySettings DifficultyManager::harderThanMiddleSettings;
DifficultySettings DifficultyManager::hardSettings;
bool DifficultyManager::initialized = false;

const DifficultySettings& DifficultyManager::getDifficultySettings(GameLevel level) {
    if (!initialized) {
        initializeDifficultyPresets();
    }
    
    switch (level) {
        case GameLevel::Easy:
            return easySettings;
        case GameLevel::HarderThanEasy:
            return harderThanEasySettings;
        case GameLevel::Middle:
            return middleSettings;
        case GameLevel::HarderThanMiddle:
            return harderThanMiddleSettings;
        case GameLevel::Hard:
            return hardSettings;
        default:
            // Fallback to easy if invalid level
            return easySettings;
    }
}

std::wstring DifficultyManager::getDifficultyDisplayName(GameLevel level) {
    switch (level) {
        case GameLevel::Easy:
            return L"Легкий";
        case GameLevel::HarderThanEasy:
            return L"Труднее, чем легкий";
        case GameLevel::Middle:
            return L"Средний";
        case GameLevel::HarderThanMiddle:
            return L"Труднее, чем средний";
        case GameLevel::Hard:
            return L"Сложный";
        default:
            return L"Неизвестный";
    }
}

void DifficultyManager::initializeDifficultyPresets() {
    // Easy difficulty - beginner friendly
    easySettings.setSnakeParameters(4.0f, 0.3f, 8.0f);  // Slow speed, slow increase
    easySettings.setItemParameters(4.0f, 3, 1.5f, 0.2f);  // Few items, long lifetime, few specials
    easySettings.setAppleChances(0.7f, 0.15f, 0.1f, 0.05f);  // Mostly red apples
    easySettings.setWallParameters(2, 0.05f);  // Few walls for testing
    easySettings.setMultipliers(1.0f, 1.2f);  // Normal score, longer effects
    
    // Harder than Easy - slight challenge
    harderThanEasySettings.setSnakeParameters(5.0f, 0.4f, 7.0f);
    harderThanEasySettings.setItemParameters(3.5f, 4, 1.3f, 0.25f);
    harderThanEasySettings.setAppleChances(0.6f, 0.2f, 0.15f, 0.05f);
    harderThanEasySettings.setWallParameters(2, 0.05f);  // Few walls
    harderThanEasySettings.setMultipliers(1.1f, 1.1f);
    
    // Middle difficulty - balanced gameplay
    middleSettings.setSnakeParameters(6.0f, 0.5f, 6.0f);  // Moderate speed and increase
    middleSettings.setItemParameters(3.0f, 5, 1.0f, 0.3f);  // Standard spawning
    middleSettings.setAppleChances(0.5f, 0.2f, 0.2f, 0.1f);  // Balanced apple types
    middleSettings.setWallParameters(4, 0.1f);  // Moderate walls
    middleSettings.setMultipliers(1.2f, 1.0f);  // Slightly better score, normal effects
    
    // Harder than Middle - challenging
    harderThanMiddleSettings.setSnakeParameters(7.5f, 0.6f, 5.0f);
    harderThanMiddleSettings.setItemParameters(2.5f, 6, 0.8f, 0.4f);  // More items, shorter lifetime
    harderThanMiddleSettings.setAppleChances(0.4f, 0.25f, 0.25f, 0.1f);  // More special apples
    harderThanMiddleSettings.setWallParameters(6, 0.15f);  // More walls
    harderThanMiddleSettings.setMultipliers(1.5f, 0.9f);  // Better score, shorter effects
    
    // Hard difficulty - expert level
    hardSettings.setSnakeParameters(9.0f, 0.8f, 4.0f);  // Fast speed, rapid increase
    hardSettings.setItemParameters(2.0f, 8, 0.6f, 0.5f);  // Frequent spawning, short lifetime
    hardSettings.setAppleChances(0.3f, 0.3f, 0.25f, 0.15f);  // Many special apples
    hardSettings.setWallParameters(8, 0.2f);  // Many walls
    hardSettings.setMultipliers(2.0f, 0.8f);  // Double score, shorter effects
    
    // Validate all settings
    if (!easySettings.validate() || !harderThanEasySettings.validate() || 
        !middleSettings.validate() || !harderThanMiddleSettings.validate() || 
        !hardSettings.validate()) {
        throw std::runtime_error("Invalid difficulty settings detected");
    }
    
    initialized = true;
}