#include "SettingStorage.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include "../SnakeSprite.hpp"

/**
 * @brief Simple JSON parser for basic key-value pairs
 * @param jsonStr JSON string to parse
 * @return map of key-value pairs
 */
std::map<std::string, std::string> SettingStorage::parseSimpleJson(const std::string& jsonStr) {
  std::map<std::string, std::string> result;

  // Remove whitespace and newlines
  std::string cleaned = jsonStr;
  cleaned.erase(std::remove_if(cleaned.begin(), cleaned.end(),
                               [](char c) { return c == ' ' || c == '\n' || c == '\r' || c == '\t'; }),
                cleaned.end());

  // Find the content between the first { and last }
  size_t start = cleaned.find('{');
  size_t end = cleaned.rfind('}');

  if (start == std::string::npos || end == std::string::npos || start >= end) {
    return result;
  }

  std::string content = cleaned.substr(start + 1, end - start - 1);

  // Split by commas and parse key-value pairs
  std::stringstream ss(content);
  std::string pair;

  while (std::getline(ss, pair, ',')) {
    size_t colonPos = pair.find(':');
    if (colonPos != std::string::npos) {
      std::string key = pair.substr(0, colonPos);
      std::string value = pair.substr(colonPos + 1);

      // Remove quotes from key and value
      if (key.front() == '"' && key.back() == '"') {
        key = key.substr(1, key.length() - 2);
      }
      if (value.front() == '"' && value.back() == '"') {
        value = value.substr(1, value.length() - 2);
      }

      result[key] = value;
    }
  }

  return result;
}

bool SettingStorage::initialize() {
  try {
    std::ifstream file(SETTINGS_FILE_PATH);

    if (!file.is_open()) {
      std::cout << "Settings file not found, creating default settings..." << std::endl;
      if (!createDefaultSettingsFile()) {
        std::cerr << "Failed to create default settings file" << std::endl;
        return false;
      }
      file.open(SETTINGS_FILE_PATH);
      if (!file.is_open()) {
        std::cerr << "Failed to open settings file after creation" << std::endl;
        return false;
      }
    }

    // get file absolute path
    std::string filePath = std::filesystem::absolute(SETTINGS_FILE_PATH).string();
    std::cout << "Settings file path: " << filePath << std::endl;

    // Read file content
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string content = buffer.str();
    std::cout << content << std::endl;
    std::map<std::string, std::string> jsonData = parseSimpleJson(content);

    // Parse settings from JSON
    if (jsonData.contains("snakeSpeed")) {
      try {
        settings.snakeSpeed = std::stoi(jsonData["snakeSpeed"]);
      } catch (const std::exception& e) {
        std::cerr << "Invalid snake speed format: " << e.what() << std::endl;
        settings.snakeSpeed = 1;
      }
    }

    if (jsonData.contains("snakeType")) {
      settings.snakeType = parseSnakeType(jsonData["snakeType"]);
    }

    if (jsonData.contains("snakeSize")) {
      try {
        settings.snakeSize = std::stoi(jsonData["snakeSize"]);
      } catch (const std::exception& e) {
        std::cerr << "Invalid snake size format: " << e.what() << std::endl;
        settings.snakeSize = 3;
      }
    }

    if (jsonData.contains("gameLevel")) {
      settings.gameLevel = parseGameLevel(jsonData["gameLevel"]);
    }

    if (jsonData.contains("gameMusic")) {
      settings.gameMusic = jsonData["gameMusic"] == "1";
    }

    if (jsonData.contains("gameSound")) {
      settings.gameSound = jsonData["gameSound"] == "1";
    }

    if (jsonData.contains("gameCountdownEnabled")) {
      settings.gameCountdownEnabled = jsonData["gameCountdownEnabled"] == "1";
    }

    if (jsonData.contains("gameCountdownInSeconds")) {
      try {
        settings.gameCountdownInSeconds = std::stoi(jsonData["gameCountdownInSeconds"]);
      } catch (const std::exception& e) {
        std::cerr << "Invalid countdown seconds format: " << e.what() << std::endl;
        settings.gameCountdownInSeconds = 3;
      }
    }

    if (jsonData.contains("gameCountdownSound")) {
      settings.gameCountdownSound = jsonData["gameCountdownSound"] == "1";
    }

    // Parse gameRecordTable (simple array parsing)
    if (jsonData.contains("gameRecordTable")) {
      std::string recordTableStr = jsonData["gameRecordTable"];
      // Remove brackets and parse numbers
      if (recordTableStr.front() == '[' && recordTableStr.back() == ']') {
        recordTableStr = recordTableStr.substr(1, recordTableStr.length() - 2);
        std::stringstream ss(recordTableStr);
        std::string score;
        settings.gameRecordTable.clear();

        while (std::getline(ss, score, ',')) {
          try {
            settings.gameRecordTable.push_back(std::stoi(score));
          } catch (const std::exception& e) {
            std::cerr << "Invalid score format: " << e.what() << std::endl;
          }
        }
      }
    }

    isInitialized = true;
    std::cout << "Settings loaded successfully" << std::endl;
    return true;

  } catch (const std::exception& e) {
    std::cerr << "Error loading settings: " << e.what() << std::endl;
    return false;
  }
}

SnakeSprite::SnakeType SettingStorage::parseSnakeType(const std::string& snakeTypeStr) {
  if (snakeTypeStr == "purple")
    return SnakeSprite::SnakeType::Purple;
  if (snakeTypeStr == "green")
    return SnakeSprite::SnakeType::Green;
  if (snakeTypeStr == "blue")
    return SnakeSprite::SnakeType::Blue;
  if (snakeTypeStr == "red")
    return SnakeSprite::SnakeType::Red;
  if (snakeTypeStr == "black")
    return SnakeSprite::SnakeType::Black;

  std::cerr << "Unknown snake type: " << snakeTypeStr << ", defaulting to purple" << std::endl;
  return SnakeSprite::SnakeType::Purple;
}

GameLevel SettingStorage::parseGameLevel(const std::string& gameLevelStr) {
  if (gameLevelStr == "easy")
    return GameLevel::Easy;
  if (gameLevelStr == "harderTheEasy")
    return GameLevel::HarderThanEasy;
  if (gameLevelStr == "middle")
    return GameLevel::Middle;
  if (gameLevelStr == "harderThenMiddle")
    return GameLevel::HarderThanMiddle;
  if (gameLevelStr == "hard")
    return GameLevel::Hard;

  std::cerr << "Unknown game level: " << gameLevelStr << ", defaulting to easy" << std::endl;
  return GameLevel::Easy;
}

bool SettingStorage::createDefaultSettingsFile() {
  try {
    std::ofstream file(SETTINGS_FILE_PATH);
    if (!file.is_open()) {
      std::cerr << "Failed to create settings file" << std::endl;
      return false;
    }

    file << "{\n";
    file << "    \"snakeSpeed\": 1,\n";
    file << "    \"snakeType\": \"purple\",\n";
    file << "    \"snakeSize\": \"3\",\n";
    file << "    \"gameLevel\": \"easy\",\n";
    file << "    \"gameMusic\": 0,\n";
    file << "    \"gameSound\": 0,\n";
    file << "    \"gameCountdownEnabled\": 1,\n";
    file << "    \"gameCountdownInSeconds\": 3,\n";
    file << "    \"gameCountdownSound\": 0\n";
    file << "}";

    file.close();

    std::cout << "Default settings file created: " << SETTINGS_FILE_PATH << std::endl;
    return true;

  } catch (const std::exception& e) {
    std::cerr << "Error creating default settings file: " << e.what() << std::endl;
    return false;
  }
}

std::string SettingStorage::snakeTypeToString(SnakeSprite::SnakeType snakeType) {
  switch (snakeType) {
    case SnakeSprite::SnakeType::Purple:
      return "purple";
    case SnakeSprite::SnakeType::Green:
      return "green";
    case SnakeSprite::SnakeType::Blue:
      return "blue";
    case SnakeSprite::SnakeType::Red:
      return "red";
    case SnakeSprite::SnakeType::Black:
      return "black";
    default:
      return "purple";
  }
}

std::string SettingStorage::gameLevelToString(GameLevel gameLevel) {
  switch (gameLevel) {
    case GameLevel::Easy:
      return "easy";
    case GameLevel::HarderThanEasy:
      return "harderTheEasy";
    case GameLevel::Middle:
      return "middle";
    case GameLevel::HarderThanMiddle:
      return "harderThenMiddle";
    case GameLevel::Hard:
      return "hard";
    default:
      return "easy";
  }
}

bool SettingStorage::addScoreToRecordTable(int score) {
  // Check if score qualifies for the record table
  if (settings.gameRecordTable.empty() || score > settings.gameRecordTable.back()) {
    // Add score and sort in descending order
    settings.gameRecordTable.push_back(score);
    std::sort(settings.gameRecordTable.begin(), settings.gameRecordTable.end(), std::greater<int>());

    // Keep only top 5 scores
    if (settings.gameRecordTable.size() > 5) {
      settings.gameRecordTable.resize(5);
    }

    // Save to file
    saveSettings();
    return true;
  }
  return false;
}

bool SettingStorage::saveSettings() {
  try {
    std::ofstream file(SETTINGS_FILE_PATH);
    if (!file.is_open()) {
      std::cerr << "Failed to open settings file for writing" << std::endl;
      return false;
    }

    file << "{\n";
    file << "    \"snakeSpeed\": " << settings.snakeSpeed << ",\n";
    file << "    \"snakeType\": \"" << snakeTypeToString(settings.snakeType) << "\",\n";
    file << "    \"snakeSize\": \"" << settings.snakeSize << "\",\n";
    file << "    \"gameLevel\": \"" << gameLevelToString(settings.gameLevel) << "\",\n";
    file << "    \"gameMusic\": " << (settings.gameMusic ? 1 : 0) << ",\n";
    file << "    \"gameSound\": " << (settings.gameSound ? 1 : 0) << ",\n";
    file << "    \"gameCountdownEnabled\": " << (settings.gameCountdownEnabled ? 1 : 0) << ",\n";
    file << "    \"gameCountdownInSeconds\": " << settings.gameCountdownInSeconds << ",\n";
    file << "    \"gameCountdownSound\": " << (settings.gameCountdownSound ? 1 : 0) << ",\n";
    file << "    \"gameRecordTable\": [";

    for (size_t i = 0; i < settings.gameRecordTable.size(); ++i) {
      file << settings.gameRecordTable[i];
      if (i < settings.gameRecordTable.size() - 1) {
        file << ", ";
      }
    }
    file << "]\n";
    file << "}";

    file.close();
    std::cout << "Settings saved successfully" << std::endl;
    return true;

  } catch (const std::exception& e) {
    std::cerr << "Error saving settings: " << e.what() << std::endl;
    return false;
  }
}
