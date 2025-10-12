#pragma once
#include <SFML/Audio.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>
#include "../Screen.hpp"
#include "../Snake.hpp"
#include "../utils/CountdownTimer.hpp"
#include "../utils/GameGrid.hpp"
#include "../utils/GameItemManager.hpp"
#include "../utils/GameUI.hpp"
#include "../utils/WallManager.hpp"

class GameScreen final : public Screen {
public:
  explicit GameScreen(sf::RenderWindow& win, Game& gameRef);

  void processEvents(const sf::Event& event) override;

  void update() override;

  void render() override;

  // Method to restart countdown (called when returning from pause)
  void restartCountdown();

  void resume();

  void pauseMusic();

  void pause();

  void unpause();

private:
  float gridSize = 824.0f;
  float scaleRelativeFactor = 912.0f / 992.0f;
  // Grid data
  GameGrid gameGrid;

  // Snake
  Snake snake;
  mutable GameUI gameUI;

  // Walls
  std::unique_ptr<WallManager> wallManager;

  // Game Items
  std::unique_ptr<GameItemManager> gameItemManager;

  // Countdown timer
  CountdownTimer countdownTimer;

  // Background music
  sf::Music* backgroundMusic;
  bool musicStarted = false;

  // Game over sound
  sf::Sound gameOverSound;
  bool gameOverSoundPlayed = false;

  // Eating apple sound
  sf::Sound eatAppleSound;

  // Start game sound
  sf::Sound startGameSound;

  // Timing
  sf::Clock moveTimer;
  sf::Clock snakeTypeTimer;
  static constexpr float SNAKE_MOVE_INTERVAL = 1.0f;         // seconds
  static constexpr float SNAKE_TYPE_CHANGE_INTERVAL = 1.0f;  // seconds

  // Snake type cycling for testing
  int currentSnakeTypeIndex = 0;
  static constexpr int TOTAL_SNAKE_TYPES = 5;  // Purple, Green, Blue, Red, Black

  // Game state
  bool gameOver = false;
  bool scoreSaved = false;
  bool isPaused = false;

  // Blinking effect for collision
  bool isBlinking = false;
  int blinkCount = 0;
  static constexpr int MAX_BLINKS = 3;
  sf::Clock blinkTimer;
  static constexpr float BLINK_DURATION = 0.5f;  // Duration of each blink state

  sf::Sprite renderBoardBorder() const;

  void renderBoardGrid() const;

  void renderDebugGrid() const;
  void handleGameOver();

  void initializeGrid();
  void updateGrid();
  void renderGameUI(const sf::Sprite& boardBorder) const;
  void startBlinking();
};
