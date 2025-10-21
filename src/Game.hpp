#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Screen.hpp"
#include "utils/SettingStorage.hpp"

namespace MenuColors {
constexpr uint8_t BACKGROUND_R = 130;
constexpr uint8_t BACKGROUND_G = 73;
constexpr uint8_t BACKGROUND_B = 113;

constexpr uint8_t TEXT_R = 251;
constexpr uint8_t TEXT_G = 242;
constexpr uint8_t TEXT_B = 54;

constexpr uint8_t BORDER_R = 171;
constexpr uint8_t BORDER_G = 99;
constexpr uint8_t BORDER_B = 148;

const sf::Color BACKGROUND_COLOR = sf::Color(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B);
const sf::Color TEXT_COLOR = sf::Color(TEXT_R, TEXT_G, TEXT_B);
const sf::Color BORDER_COLOR = sf::Color(BORDER_R, BORDER_G, BORDER_B);
}  // namespace MenuColors

class Game {
private:
  sf::RenderWindow& window;
  bool isRunning;
  Screen* currentScreen;
  Screen* previousScreen;
  SettingStorage settingStorage;

  int score = -1;
  int highScore = -1;
  bool isPaused = false;

  static constexpr bool DEBUG_UI_TEXT = false;

  void processEvents(const sf::Event& event) const;

public:
  explicit Game(sf::RenderWindow& window);
  ~Game();

  void start() const;

  void setCurrentScreen(Screen* screen);
  void setCurrentScreenWithPrevious(Screen* screen, Screen* previous);

  [[nodiscard]] int getScore() const { return score; }
  [[nodiscard]] int getHighScore() const { return highScore; }
  [[nodiscard]] bool getIsPaused() const { return isPaused; }
  [[nodiscard]] sf::RenderWindow& getWindow() const { return window; }

  void setScore(int newScore) { score = newScore; }
  void setHighScore(int newHighScore) { highScore = newHighScore; }
  void setIsPaused(bool paused) { isPaused = paused; }

  [[nodiscard]] const SettingStorage& getSettingsReader() const { return settingStorage; }

  void loadSettings() { settingStorage.loadSettings(); }

  void addScore(int points) { score += points; }
  void resetScore() { score = 0; }
  void updateHighScore() {
    if (score > highScore)
      highScore = score;
  }

  void saveScoreToRecordTable();
  bool isNewHighScore() const;

  void returnToPreviousScreen();
  void returnToGameScreen();
};
