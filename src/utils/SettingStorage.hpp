#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include "../SnakeSprite.hpp"

using json = nlohmann::json;

enum class GameDifficultyLevel { Easy, HarderThanEasy, Middle, HarderThanMiddle, Hard };

struct GameSettings {
  int snakeSpeed = 1;
  SnakeSprite::SnakeType snakeType = SnakeSprite::SnakeType::Purple;
  int snakeSize = 3;
  GameDifficultyLevel gameDifficultyLevel = GameDifficultyLevel::Hard;
  bool gameMusic = true;
  bool gameSound = true;
  int gameCountdownInSeconds = 3;
  std::vector<int> gameRecordTable = {0};

  json toJson() const;
  void fromJson(const json& j);
};

class SettingStorage {
private:
  static constexpr const char* SETTINGS_FILE_PATH = "settings.json";
  GameSettings settings;
  bool isInitialized = false;

public:
  SettingStorage() = default;

  ~SettingStorage() = default;

  static SnakeSprite::SnakeType parseSnakeType(const std::string& snakeTypeStr);

  static GameDifficultyLevel parseGameDifficultyLevel(const std::string& gameDifficultyLevelStr);

  static bool createDefaultSettingsFile();

  bool loadSettings();

  [[nodiscard]] bool getIsInitialized() const { return isInitialized; }

  [[nodiscard]] int getSnakeSpeed() const { return settings.snakeSpeed; }

  [[nodiscard]] SnakeSprite::SnakeType getSnakeType() const { return settings.snakeType; }

  [[nodiscard]] int getSnakeSize() const { return settings.snakeSize; }

  [[nodiscard]] GameDifficultyLevel getGameDifficultyLevel() const { return settings.gameDifficultyLevel; }

  [[nodiscard]] bool getGameMusic() const { return settings.gameMusic; }

  [[nodiscard]] bool getGameSound() const { return settings.gameSound; }

  [[nodiscard]] int getGameCountdownInSeconds() const { return settings.gameCountdownInSeconds; }

  [[nodiscard]] const std::vector<int>& getGameRecordTable() const { return settings.gameRecordTable; }

  bool addScoreToRecordTable(int score);

  bool saveSettings() const;

  static std::string snakeTypeToString(SnakeSprite::SnakeType snakeType);

  static std::string gameDifficultyLevelToString(GameDifficultyLevel gameDifficultyLevel);

  void setGameDifficultyLevel(GameDifficultyLevel gameDifficultyLevel) {
    settings.gameDifficultyLevel = gameDifficultyLevel;
  }

  void setSnakeType(SnakeSprite::SnakeType snakeType) { settings.snakeType = snakeType; }
  void setSnakeSpeed(int snakeSpeed) { settings.snakeSpeed = snakeSpeed; }
  void setSnakeSize(int snakeSize) { settings.snakeSize = snakeSize; }
  void setGameMusic(bool gameMusic) { settings.gameMusic = gameMusic; }
  void setGameSound(bool gameSound) { settings.gameSound = gameSound; }

  void setGameCountdownInSeconds(int gameCountdownInSeconds) {
    settings.gameCountdownInSeconds = gameCountdownInSeconds;
  }

  void setGameRecordTable(const std::vector<int>& gameRecordTable) { settings.gameRecordTable = gameRecordTable; }
};
