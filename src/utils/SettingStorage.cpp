#include "SettingStorage.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include "../SnakeSprite.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool SettingStorage::loadSettings() {
  try {
    std::ifstream file(SETTINGS_FILE_PATH);

    if (!file.is_open()) {
      std::cout << "Settings file not found at: " << SETTINGS_FILE_PATH << std::endl;
      std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
      std::cout << "Creating default settings..." << std::endl;
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

    std::string filePath = std::filesystem::absolute(SETTINGS_FILE_PATH).string();
    std::cout << "Settings file path: " << filePath << std::endl;

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string content = buffer.str();
    std::cout << content << std::endl;

    try {
      json jsonData = json::parse(content);
      std::cout << "JSON parsed successfully" << std::endl;

      for (auto& [key, value] : jsonData.items()) {
        std::cout << "Key: " << key << ", Type: " << value.type_name() << std::endl;
      }

      try {
        settings.fromJson(jsonData);
        isInitialized = true;
        return true;
      } catch (const json::exception& e) {
        std::cerr << "Error in fromJson: " << e.what() << std::endl;
        return false;
      }
    } catch (const json::exception& e) {
      std::cerr << "Error parsing JSON: " << e.what() << std::endl;
      return false;
    }
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

GameDifficultyLevel SettingStorage::parseGameDifficultyLevel(const std::string& gameDifficultyLevelStr) {
  if (gameDifficultyLevelStr == "easy")
    return GameDifficultyLevel::Easy;
  if (gameDifficultyLevelStr == "harderTheEasy")
    return GameDifficultyLevel::HarderThanEasy;
  if (gameDifficultyLevelStr == "middle")
    return GameDifficultyLevel::Middle;
  if (gameDifficultyLevelStr == "harderThenMiddle")
    return GameDifficultyLevel::HarderThanMiddle;
  if (gameDifficultyLevelStr == "hard")
    return GameDifficultyLevel::Hard;

  std::cerr << "Unknown game difficulty level: " << gameDifficultyLevelStr << ", defaulting to easy" << std::endl;
  return GameDifficultyLevel::Easy;
}

bool SettingStorage::createDefaultSettingsFile() {
  try {
    std::ofstream file(SETTINGS_FILE_PATH);
    if (!file.is_open()) {
      std::cerr << "Failed to create settings file" << std::endl;
      return false;
    }

    file << R"({)";
    file << R"(    "snakeSpeed": 1,\n)";
    file << R"(    "snakeType": "purple",\n)";
    file << R"(    "snakeSize": 3,\n)";
    file << R"(    "gameDifficultyLevel": "easy",\n)";
    file << R"(    "gameMusic": true,\n)";
    file << R"(    "gameSound": true,\n)";
    file << R"(    "gameCountdownInSeconds": 3,\n)";
    file << R"(    "gameRecordTable": [0]\n)";
    file << R"(})";

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

std::string SettingStorage::gameDifficultyLevelToString(GameDifficultyLevel gameDifficultyLevel) {
  switch (gameDifficultyLevel) {
    case GameDifficultyLevel::Easy:
      return "easy";
    case GameDifficultyLevel::HarderThanEasy:
      return "harderTheEasy";
    case GameDifficultyLevel::Middle:
      return "middle";
    case GameDifficultyLevel::HarderThanMiddle:
      return "harderThenMiddle";
    case GameDifficultyLevel::Hard:
      return "hard";
    default:
      return "easy";
  }
}

bool SettingStorage::addScoreToRecordTable(int score) {
  loadSettings();

  if (settings.gameRecordTable.empty() || score > settings.gameRecordTable.back()) {
    settings.gameRecordTable.push_back(score);
    std::ranges::sort(settings.gameRecordTable, std::ranges::greater());

    if (settings.gameRecordTable.size() > 5) {
      settings.gameRecordTable.resize(5);
    }

    saveSettings();
    return true;
  }
  return false;
}

bool SettingStorage::saveSettings() const {
  try {
    std::ofstream file(SETTINGS_FILE_PATH);
    if (!file.is_open()) {
      std::cerr << "Failed to open settings file for writing" << std::endl;
      return false;
    }

    json j = settings.toJson();
    file << j.dump(4);
    file.close();
    std::cout << "Settings saved successfully" << std::endl;
    return true;

  } catch (const std::exception& e) {
    std::cerr << "Error saving settings: " << e.what() << std::endl;
    return false;
  }
}

json GameSettings::toJson() const {
  json j;
  j["snakeSpeed"] = snakeSpeed;
  j["snakeType"] = SettingStorage::snakeTypeToString(snakeType);
  j["snakeSize"] = snakeSize;
  j["gameDifficultyLevel"] = SettingStorage::gameDifficultyLevelToString(gameDifficultyLevel);
  j["gameMusic"] = gameMusic;
  j["gameSound"] = gameSound;
  j["gameCountdownInSeconds"] = gameCountdownInSeconds;
  j["gameRecordTable"] = gameRecordTable;
  return j;
}

void GameSettings::fromJson(const json& j) {
  try {
    if (j.contains("snakeSpeed")) {
      snakeSpeed = j.value("snakeSpeed", 1);
    }

    if (j.contains("snakeType")) {
      std::string snakeTypeStr;
      if (j["snakeType"].is_string()) {
        snakeTypeStr = j["snakeType"].get<std::string>();
      } else {
        snakeTypeStr = std::to_string(j["snakeType"].get<int>());
      }
      snakeType = SettingStorage::parseSnakeType(snakeTypeStr);
    }

    if (j.contains("snakeSize")) {
      if (j["snakeSize"].is_string()) {
        snakeSize = std::stoi(j.value("snakeSize", "3"));
      } else {
        snakeSize = j.value("snakeSize", 3);
      }
    }

    if (j.contains("gameDifficultyLevel")) {
      std::string difficultyStr;
      if (j["gameDifficultyLevel"].is_string()) {
        difficultyStr = j["gameDifficultyLevel"].get<std::string>();
      } else {
        difficultyStr = std::to_string(j["gameDifficultyLevel"].get<int>());
      }
      gameDifficultyLevel = SettingStorage::parseGameDifficultyLevel(difficultyStr);
    }

    if (j.contains("gameMusic")) {
      if (j["gameMusic"].is_boolean()) {
        gameMusic = j["gameMusic"].get<bool>();
      } else {
        gameMusic = j["gameMusic"].get<int>() != 0;
      }
    }

    if (j.contains("gameSound")) {
      if (j["gameSound"].is_boolean()) {
        gameSound = j["gameSound"].get<bool>();
      } else {
        gameSound = j["gameSound"].get<int>() != 0;
      }
    }

    if (j.contains("gameCountdownInSeconds")) {
      gameCountdownInSeconds = j.value("gameCountdownInSeconds", 3);
    }

    if (j.contains("gameRecordTable")) {
      gameRecordTable = j.value("gameRecordTable", std::vector<int>{0});
    }
  } catch (const json::exception& e) {
    std::cerr << "JSON exception in fromJson: " << e.what() << std::endl;
    throw;
  } catch (const std::exception& e) {
    std::cerr << "Standard exception in fromJson: " << e.what() << std::endl;
    throw;
  }
}
