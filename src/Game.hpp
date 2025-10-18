#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Screen.hpp"
#include "utils/SettingStorage.hpp"

class Game {
private:
  sf::RenderWindow& window;
  bool isRunning;
  Screen* currentScreen;
  Screen* previousScreen;  // Store previous screen for pause functionality
  SettingStorage settingStorage;

  // Game state variables (moved from GameState)
  int score = 0;
  int highScore = 0;
  GameLevel gameLevel = GameLevel::Easy;
  bool isPaused = false;

  // Debug settings
  static constexpr bool DEBUG_UI_TEXT = false;

  void processEvents(const sf::Event& event) const;

public:
  explicit Game(sf::RenderWindow& window);
  ~Game() = default;

  void start() const;

  void setCurrentScreen(Screen* screen);
  void setCurrentScreenWithPrevious(Screen* screen, Screen* previous);

  // Game state getters (moved from GameState)
  [[nodiscard]] int getScore() const { return score; }
  [[nodiscard]] int getHighScore() const { return highScore; }
  [[nodiscard]] GameLevel getGameLevel() const { return gameLevel; }
  [[nodiscard]] bool getIsPaused() const { return isPaused; }
  [[nodiscard]] sf::RenderWindow& getWindow() const { return window; }

  // Game state setters (moved from GameState)
  void setScore(int newScore) { score = newScore; }
  void setHighScore(int newHighScore) { highScore = newHighScore; }
  void setGameLevel(GameLevel newGameLevel) { gameLevel = newGameLevel; }
  void setIsPaused(bool paused) { isPaused = paused; }

  // Settings accessors
  [[nodiscard]] const SettingStorage& getSettingsReader() const { return settingStorage; }
  [[nodiscard]] const GameSettings& getGameSettings() const { return settingStorage.getSettings(); }

  // Game logic methods (moved from GameState)
  void addScore(int points) { score += points; }
  void resetScore() { score = 0; }
  void updateHighScore() {
    if (score > highScore)
      highScore = score;
  }

  // Record table management
  void saveScoreToRecordTable();
  bool isNewHighScore() const;

  // Screen management
  void returnToPreviousScreen();
  void returnToGameScreen();
};
