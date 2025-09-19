#pragma once
#include <SFML/Graphics.hpp>
#include "Screen.hpp"

class Game {
private:
  sf::RenderWindow& window;
  bool isRunning;
  Screen* currentScreen;

  // Game state variables (moved from GameState)
  int score = 0;
  int highScore = 0;
  int difficulty = 1;
  bool isPaused = false;

  // Debug settings
  static constexpr bool DEBUG_UI_TEXT = true;

  void processEvents(const sf::Event& event) const;

public:
  explicit Game(sf::RenderWindow& window);
  ~Game() = default;

  void start() const;

  void setCurrentScreen(Screen* screen);

  // Game state getters (moved from GameState)
  [[nodiscard]] int getScore() const { return score; }
  [[nodiscard]] int getHighScore() const { return highScore; }
  [[nodiscard]] int getDifficulty() const { return difficulty; }
  [[nodiscard]] bool getIsPaused() const { return isPaused; }
  [[nodiscard]] sf::RenderWindow& getWindow() const { return window; }

  // Game state setters (moved from GameState)
  void setScore(int newScore) { score = newScore; }
  void setHighScore(int newHighScore) { highScore = newHighScore; }
  void setDifficulty(int newDifficulty) { difficulty = newDifficulty; }
  void setIsPaused(bool paused) { isPaused = paused; }

  // Game logic methods (moved from GameState)
  void addScore(int points) { score += points; }
  void resetScore() { score = 0; }
  void updateHighScore() {
    if (score > highScore)
      highScore = score;
  }
};
