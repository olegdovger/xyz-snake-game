#pragma once

#include <map>
#include <string>
#include <vector>
#include "../SnakeSprite.hpp"

/**
 * @brief Enumeration for game difficulty levels
 */
enum class GameLevel { Easy, HarderThanEasy, Middle, HarderThanMiddle, Hard };

/**
 * @brief Structure to hold all game settings
 */
struct GameSettings {
  int snakeSpeed = 1;
  SnakeSprite::SnakeType snakeType = SnakeSprite::SnakeType::Purple;
  int snakeSize = 3;
  GameLevel gameLevel = GameLevel::Hard;
  bool gameMusic = true;
  bool gameSound = true;
  bool gameCountdownEnabled = true;
  int gameCountdownInSeconds = 3;
  bool gameCountdownSound = false;
  std::vector<int> gameRecordTable = {0};
};

/**
 * @brief Class responsible for reading and managing game settings from JSON file
 */
class SettingStorage {
private:
  static constexpr const char* SETTINGS_FILE_PATH = "settings.json";
  GameSettings settings;
  bool isInitialized = false;

  /**
     * @brief Parse snake type from string
     * @param snakeTypeStr String representation of snake type
     * @return SnakeSprite::SnakeType enum value
     */
  static SnakeSprite::SnakeType parseSnakeType(const std::string& snakeTypeStr);

  /**
     * @brief Parse game level from string
     * @param gameLevelStr String representation of game level
     * @return GameLevel enum value
     */
  static GameLevel parseGameLevel(const std::string& gameLevelStr);

  /**
     * @brief Create default settings file
     * @return true if successful, false otherwise
     */
  static bool createDefaultSettingsFile();

  /**
     * @brief Simple JSON parser for basic key-value pairs
     * @param jsonStr JSON string to parse
     * @return map of key-value pairs
     */
  static std::map<std::string, std::string> parseSimpleJson(const std::string& jsonStr);

public:
  /**
     * @brief Default constructor
     */
  SettingStorage() = default;

  /**
     * @brief Destructor
     */
  ~SettingStorage() = default;

  /**
     * @brief Initialize settings by reading from JSON file
     * @return true if successful, false otherwise
     */
  bool initialize();

  /**
     * @brief Check if settings are initialized
     * @return true if initialized, false otherwise
     */
  [[nodiscard]] bool getIsInitialized() const { return isInitialized; }

  /**
     * @brief Get current game settings
     * @return const reference to GameSettings
     */
  [[nodiscard]] const GameSettings& getSettings() const { return settings; }

  /**
     * @brief Get snake speed setting
     * @return snake speed value
     */
  [[nodiscard]] int getSnakeSpeed() const { return settings.snakeSpeed; }

  /**
     * @brief Get snake type setting
     * @return snake type enum
     */
  [[nodiscard]] SnakeSprite::SnakeType getSnakeType() const { return settings.snakeType; }

  /**
     * @brief Get snake size setting
     * @return snake size value
     */
  [[nodiscard]] int getSnakeSize() const { return settings.snakeSize; }

  /**
     * @brief Get game level setting
     * @return game level enum
     */
  [[nodiscard]] GameLevel getGameLevel() const { return settings.gameLevel; }

  /**
     * @brief Get game music setting
     * @return true if music is enabled, false otherwise
     */
  [[nodiscard]] bool getGameMusic() const { return settings.gameMusic; }

  /**
     * @brief Get game sound setting
     * @return true if sound is enabled, false otherwise
     */
  [[nodiscard]] bool getGameSound() const { return settings.gameSound; }

  /**
     * @brief Get game countdown enabled setting
     * @return true if countdown is enabled, false otherwise
     */
  [[nodiscard]] bool getGameCountdownEnabled() const { return settings.gameCountdownEnabled; }

  /**
     * @brief Get game countdown duration in seconds
     * @return countdown duration in seconds
     */
  [[nodiscard]] int getGameCountdownInSeconds() const { return settings.gameCountdownInSeconds; }

  /**
     * @brief Get game countdown sound setting
     * @return true if countdown sound is enabled, false otherwise
     */
  [[nodiscard]] bool getGameCountdownSound() const { return settings.gameCountdownSound; }

  /**
     * @brief Get game record table
     * @return const reference to record table vector
     */
  [[nodiscard]] const std::vector<int>& getGameRecordTable() const { return settings.gameRecordTable; }

  /**
     * @brief Add score to record table if it qualifies
     * @param score Score to potentially add
     * @return true if score was added, false otherwise
     */
  bool addScoreToRecordTable(int score);

  /**
     * @brief Save settings to file
     * @return true if successful, false otherwise
     */
  bool saveSettings();

  /**
     * @brief Convert snake type to string
     * @param snakeType Snake type enum
     * @return string representation
     */
  static std::string snakeTypeToString(SnakeSprite::SnakeType snakeType);

  /**
     * @brief Convert game level to string
     * @param gameLevel Game level enum
     * @return string representation
     */
  static std::string gameLevelToString(GameLevel gameLevel);

  void setGameLevel(GameLevel gameLevel) { settings.gameLevel = gameLevel; }
  void setSnakeType(SnakeSprite::SnakeType snakeType) { settings.snakeType = snakeType; }
  void setSnakeSpeed(int snakeSpeed) { settings.snakeSpeed = snakeSpeed; }
  void setSnakeSize(int snakeSize) { settings.snakeSize = snakeSize; }
  void setGameMusic(bool gameMusic) { settings.gameMusic = gameMusic; }
  void setGameSound(bool gameSound) { settings.gameSound = gameSound; }
  void setGameCountdownEnabled(bool gameCountdownEnabled) { settings.gameCountdownEnabled = gameCountdownEnabled; }
  void setGameCountdownInSeconds(int gameCountdownInSeconds) {
    settings.gameCountdownInSeconds = gameCountdownInSeconds;
  }
  void setGameCountdownSound(bool gameCountdownSound) { settings.gameCountdownSound = gameCountdownSound; }
  void setGameRecordTable(const std::vector<int>& gameRecordTable) { settings.gameRecordTable = gameRecordTable; }
};
