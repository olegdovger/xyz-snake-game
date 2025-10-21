#include "DifficultyManager.hpp"
#include <stdexcept>

DifficultySettings DifficultyManager::easySettings;
DifficultySettings DifficultyManager::harderThanEasySettings;
DifficultySettings DifficultyManager::middleSettings;
DifficultySettings DifficultyManager::harderThanMiddleSettings;
DifficultySettings DifficultyManager::hardSettings;
bool DifficultyManager::initialized = false;

const DifficultySettings& DifficultyManager::getDifficultySettings(GameDifficultyLevel difficultyLevel) {
  if (!initialized) {
    initializeDifficultyPresets();
  }

  switch (difficultyLevel) {
    case GameDifficultyLevel::Easy:
      return easySettings;
    case GameDifficultyLevel::HarderThanEasy:
      return harderThanEasySettings;
    case GameDifficultyLevel::Middle:
      return middleSettings;
    case GameDifficultyLevel::HarderThanMiddle:
      return harderThanMiddleSettings;
    case GameDifficultyLevel::Hard:
      return hardSettings;
    default:
      return easySettings;
  }
}

std::wstring DifficultyManager::getDifficultyDisplayName(GameDifficultyLevel difficultyLevel) {
  switch (difficultyLevel) {
    case GameDifficultyLevel::Easy:
      return L"Легкий";
    case GameDifficultyLevel::HarderThanEasy:
      return L"Труднее, чем легкий";
    case GameDifficultyLevel::Middle:
      return L"Средний";
    case GameDifficultyLevel::HarderThanMiddle:
      return L"Труднее, чем средний";
    case GameDifficultyLevel::Hard:
      return L"Сложный";
    default:
      return L"Неизвестный";
  }
}

void DifficultyManager::initializeDifficultyPresets() {
  easySettings.setSnakeParameters(4.0f, 0.3f, 8.0f);
  easySettings.setItemParameters(4.0f, 3, 1.5f, 0.2f);
  easySettings.setAppleChances(0.7f, 0.15f, 0.1f, 0.05f);
  easySettings.setWallParameters(2, 0.05f);
  easySettings.setMultipliers(1.0f, 1.2f);

  harderThanEasySettings.setSnakeParameters(5.0f, 0.4f, 7.0f);
  harderThanEasySettings.setItemParameters(3.5f, 4, 1.3f, 0.25f);
  harderThanEasySettings.setAppleChances(0.6f, 0.2f, 0.15f, 0.05f);
  harderThanEasySettings.setWallParameters(2, 0.05f);
  harderThanEasySettings.setMultipliers(1.1f, 1.1f);

  middleSettings.setSnakeParameters(6.0f, 0.5f, 6.0f);
  middleSettings.setItemParameters(3.0f, 5, 1.0f, 0.3f);
  middleSettings.setAppleChances(0.5f, 0.2f, 0.2f, 0.1f);
  middleSettings.setWallParameters(4, 0.1f);
  middleSettings.setMultipliers(1.2f, 1.0f);

  harderThanMiddleSettings.setSnakeParameters(7.5f, 0.6f, 5.0f);
  harderThanMiddleSettings.setItemParameters(2.5f, 6, 0.8f, 0.4f);
  harderThanMiddleSettings.setAppleChances(0.4f, 0.25f, 0.25f, 0.1f);
  harderThanMiddleSettings.setWallParameters(6, 0.15f);
  harderThanMiddleSettings.setMultipliers(1.5f, 0.9f);

  hardSettings.setSnakeParameters(9.0f, 0.8f, 4.0f);
  hardSettings.setItemParameters(2.0f, 8, 0.6f, 0.5f);
  hardSettings.setAppleChances(0.3f, 0.3f, 0.25f, 0.15f);
  hardSettings.setWallParameters(8, 0.2f);
  hardSettings.setMultipliers(2.0f, 0.8f);

  if (!easySettings.validate() || !harderThanEasySettings.validate() || !middleSettings.validate() ||
      !harderThanMiddleSettings.validate() || !hardSettings.validate()) {
    throw std::runtime_error("Invalid difficulty settings detected");
  }

  initialized = true;
}