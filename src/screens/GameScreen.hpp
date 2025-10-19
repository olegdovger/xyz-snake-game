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
  ~GameScreen();

  void processEvents(const sf::Event& event) override;

  void update() override;

  void render() override;

  void restartCountdown();

  void resume();

  void pauseMusic();
  void resumeMusic();

  void pause();

  void unpause();

private:
  float gridSize = 824.0f;
  float scaleRelativeFactor = 912.0f / 992.0f;
  GameGrid gameGrid;

  Snake snake;
  mutable GameUI gameUI;

  std::unique_ptr<WallManager> wallManager;

  std::unique_ptr<GameItemManager> gameItemManager;

  const DifficultySettings* difficultySettings;

  CountdownTimer countdownTimer;

  sf::Music* backgroundMusic;
  bool musicStarted = false;

  sf::Sound gameOverSound;
  bool gameOverSoundPlayed = false;

  sf::Sound eatAppleSound;

  sf::Sound startGameSound;

  bool soundEnabled = true;
  bool musicEnabled = true;

  sf::Clock moveTimer;
  sf::Clock snakeTypeTimer;
  sf::Clock speedIncreaseTimer;

  // Game state
  bool gameOver = false;
  bool scoreSaved = false;
  bool isPaused = false;

  // Blinking effect for collision
  bool isBlinking = false;
  int blinkCount = 0;
  static constexpr int MAX_BLINKS = 3;
  sf::Clock blinkTimer;
  static constexpr float BLINK_DURATION = 0.5f;

  sf::Sprite renderBoardBorder() const;

  void renderBoardGrid() const;

  void renderDebugGrid() const;
  void handleGameOver();

  void initializeGrid();
  void updateGrid();
  void renderGameUI(const sf::Sprite& boardBorder) const;
  void startBlinking();
};
