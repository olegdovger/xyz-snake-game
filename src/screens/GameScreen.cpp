#include "GameScreen.hpp"
#include "../config/AudioConstants.hpp"
#include "../utils/GameItem.hpp"
#include "../utils/GameItemManager.hpp"
#include "../utils/GameUI.hpp"
#include "../utils/ResourceLoader.hpp"
#include "../utils/ScalingUtils.hpp"
#include "../utils/SettingStorage.hpp"
#include "../utils/WallManager.hpp"
#include "../utils/difficulty/DifficultyManager.hpp"
#include "HighScores.hpp"
#include "PauseScreen.hpp"

using namespace shape;

GameScreen::GameScreen(sf::RenderWindow& win, Game& gameRef)
    : Screen(win, gameRef),
      gameGrid(32, 32, 824.0f, sf::Vector2f(0, 0), 1.0f, 912.0f),
      snake(sf::Vector2i(16, 16), 5),
      countdownTimer(1, false),
      gameOverSound(ResourceLoader::getSound(SoundType::GameOver)),
      eatAppleSound(ResourceLoader::getSound(SoundType::EatApple)),
      startGameSound(ResourceLoader::getSound(SoundType::StartGame)) {
  initializeGrid();

  backgroundMusic = &ResourceLoader::getMusic(MusicType::BackgroundMusic);
  backgroundMusic->setLooping(true);
  backgroundMusic->setVolume(AudioConstants::Music::BACKGROUND_MUSIC_VOLUME);

  gameOverSound.setVolume(AudioConstants::SoundEffects::GAME_OVER_VOLUME);
  gameOverSoundPlayed = false;

  eatAppleSound.setVolume(AudioConstants::SoundEffects::EAT_APPLE_VOLUME);
  startGameSound.setVolume(AudioConstants::SoundEffects::START_GAME_VOLUME);

  game.loadSettings();

  soundEnabled = game.getSettingsReader().getGameSound();
  musicEnabled = game.getSettingsReader().getGameMusic();

  snake.setSnakeType(game.getSettingsReader().getSnakeType());

  this->difficultySettings =
      &DifficultyManager::getDifficultySettings(game.getSettingsReader().getGameDifficultyLevel());

  snake.setSpeed(this->difficultySettings->getBaseSnakeSpeed());

  countdownTimer.setSoundEnabled(soundEnabled);

  countdownTimer.setDuration(game.getSettingsReader().getGameCountdownInSeconds());
  countdownTimer.start();

  snakeTypeTimer.restart();

  gameUI = GameUI();

  game.resetScore();
  gameUI.setScore(0);
  gameUI.setApples(0);

  wallManager = std::make_unique<WallManager>(gameGrid, *difficultySettings);

  gameItemManager = std::make_unique<GameItemManager>(gameGrid, *this->difficultySettings);

  int wallsGenerated = 0;
  int maxAttempts = difficultySettings->getWallCount() * 3;
  int attempts = 0;

  while (wallsGenerated < difficultySettings->getWallCount() && attempts < maxAttempts) {
    if (wallManager->tryGenerateWall(snake)) {
      wallsGenerated++;
    }
    attempts++;
  }
}

GameScreen::~GameScreen() {
  pauseMusic();
}

void GameScreen::processEvents(const sf::Event& event) {

  if (event.is<sf::Event::KeyPressed>()) {
    switch (event.getIf<sf::Event::KeyPressed>()->code) {
      case sf::Keyboard::Key::Escape:
        pause();
        pauseMusic();
        game.setCurrentScreenWithPrevious(new PauseScreen(window, game), this);
        break;
      case sf::Keyboard::Key::P:
        if (!gameOver) {
          pause();
          pauseMusic();
          game.setCurrentScreenWithPrevious(new PauseScreen(window, game), this);
        }
        break;
      case sf::Keyboard::Key::Up:
      case sf::Keyboard::Key::W:
        if (!gameOver) {
          snake.setDirection(Snake::Direction::Up);
        }
        break;
      case sf::Keyboard::Key::Down:
      case sf::Keyboard::Key::S:
        if (!gameOver) {
          snake.setDirection(Snake::Direction::Down);
        }
        break;
      case sf::Keyboard::Key::Left:
      case sf::Keyboard::Key::A:
        if (!gameOver) {
          snake.setDirection(Snake::Direction::Left);
        }
        break;
      case sf::Keyboard::Key::Right:
      case sf::Keyboard::Key::D:
        if (!gameOver) {
          snake.setDirection(Snake::Direction::Right);
        }
        break;
      default:
        break;
    }
  }

  if (event.is<sf::Event::Resized>()) {
    initializeGrid();
  }
}

void GameScreen::update() {
  if (isPaused) {
    return;
  }

  if (wallManager) {
    wallManager->update(1.0f / 60.0f, snake);
  }

  if (gameItemManager && countdownTimer.getIsFinished()) {
    gameItemManager->update(1.0f / 60.0f, snake);
  }

  if (isBlinking) {
    if (blinkTimer.getElapsedTime().asSeconds() >= BLINK_DURATION) {
      blinkCount++;
      blinkTimer.restart();

      if (blinkCount >= MAX_BLINKS) {
        isBlinking = false;
        blinkCount = 0;
        handleGameOver();
      }
    }
  }

  if (countdownTimer.getIsFinished() && !musicStarted && !isBlinking) {
    if (musicEnabled) {
      backgroundMusic->play();
    }
    musicStarted = true;
    if (soundEnabled) {
      startGameSound.play();
    }
  }

  if (countdownTimer.getIsFinished() && !isBlinking) {
    if (speedIncreaseTimer.getElapsedTime().asSeconds() >= difficultySettings->getSpeedIncreaseInterval()) {
      float currentSpeed = snake.getSpeed();
      snake.setSpeed(currentSpeed + difficultySettings->getSpeedIncreaseRate());
      speedIncreaseTimer.restart();
    }

    if (moveTimer.getElapsedTime().asSeconds() >= 1 / snake.getSpeed()) {
      snake.move();

      if (gameItemManager) {
        auto* collidedItem = gameItemManager->checkCollision(snake.getHead());
        if (collidedItem) {
          if (soundEnabled) {
            eatAppleSound.play();
          }

          collidedItem->applySpecialEffects(snake);

          snake.grow();

          int points = static_cast<int>(collidedItem->getPoints() * difficultySettings->getScoreMultiplier());
          game.addScore(points);
          gameUI.setScore(game.getScore());
          gameUI.setApples(gameUI.getApples() + 1);

          gameItemManager->removeItem(collidedItem);
        }
      }

      bool wallCollision = false;
      if (wallManager && !snake.isInvincible()) {
        wallCollision = wallManager->checkWallCollision(snake.getHead());
      }

      if (wallCollision || snake.checkWallCollision(gameGrid.getCols(), gameGrid.getRows()) ||
          snake.checkSelfCollision()) {
        pauseMusic();

        if (!gameOverSoundPlayed) {
          if (soundEnabled) {
            gameOverSound.play();
          }
          gameOverSoundPlayed = true;
        }

        snake.kill();
        startBlinking();
      }

      moveTimer.restart();
    }
  }

  countdownTimer.update();
}

sf::Sprite GameScreen::renderBoardBorder() const {
  const auto texture = ResourceLoader::getTexture(TextureType::BoardBorder);
  sf::Sprite sprite(texture);

  const float scale = getScale(sf::Vector2f(sprite.getTexture().getSize()), window.getSize());

  sprite.setScale(sf::Vector2f(scale, scale));

  const auto position = getPosition(sf::Vector2f(sprite.getTexture().getSize()), window.getSize(), scale);

  sprite.setPosition(position);

  window.draw(sprite);

  return sprite;
}

void GameScreen::renderBoardGrid() const {
  const auto texture = ResourceLoader::getTexture(TextureType::BoardGrid);
  sf::Sprite sprite(texture);

  const float scaleRelativeFactor = 912.0f / 992.0f;
  const float scale = getScale(sf::Vector2f(sprite.getTexture().getSize()), window.getSize()) * scaleRelativeFactor;

  sprite.setScale(sf::Vector2f(scale, scale));
  const auto position = getPosition(sf::Vector2f(sprite.getTexture().getSize()), window.getSize(), scale);

  sprite.setPosition(position);

  window.draw(sprite);
}

void GameScreen::renderDebugGrid() const {
  sf::RectangleShape border(sf::Vector2f(gameGrid.getGridBounds().size.x, gameGrid.getGridBounds().size.y));
  border.setFillColor(sf::Color::Transparent);
  border.setOutlineColor(sf::Color(255, 255, 255, 128));
  border.setOutlineThickness(8.0f * gameGrid.getScale());

  border.setPosition(gameGrid.getTopLeft());

  window.draw(border);

  sf::RectangleShape cell(sf::Vector2f(gameGrid.getCellSize(), gameGrid.getCellSize()));
  cell.setScale(sf::Vector2f(gameGrid.getScale(), gameGrid.getScale()));
  cell.setFillColor(sf::Color::Transparent);
  cell.setOutlineColor(sf::Color(0, 255, 0, 128));
  cell.setOutlineThickness(1.0f * gameGrid.getScale());

  for (int row = 0; row < gameGrid.getRows(); ++row) {
    for (int col = 0; col < gameGrid.getCols(); ++col) {
      cell.setPosition(gameGrid.getCellPosition(row, col));
      window.draw(cell);
    }
  }
}

void GameScreen::render() {
  const auto boardBorder = renderBoardBorder();
  renderBoardGrid();

  renderGameUI(boardBorder);

  if (wallManager) {
    wallManager->render(window, gameGrid);
  }

  if (gameItemManager) {
    gameItemManager->render(window, gameGrid);
  }

  snake.setBlinking(isBlinking);

  snake.render(window, gameGrid);

  if (countdownTimer.getIsActive()) {
    sf::Vector2u windowSize = window.getSize();
    countdownTimer.setPosition(sf::Vector2f(windowSize.x / 2.0f, windowSize.y / 2.0f));
    countdownTimer.render(window);
  }
}

void GameScreen::renderGameUI(const sf::Sprite& boardBorder) const {
  const auto boardBorderSize = boardBorder.getLocalBounds().size;
  const auto position =
      getPosition(sf::Vector2f(boardBorder.getTexture().getSize()), window.getSize(), boardBorder.getScale().x);

  gameUI.setScale(boardBorder.getScale().x);
  gameUI.setSpeed(static_cast<int>(snake.getSpeed()));
  gameUI.render(window, sf::Vector2f(boardBorder.getGlobalBounds().position.x +
                                         (boardBorderSize.x + 16) * boardBorder.getScale().x,
                                     boardBorder.getGlobalBounds().position.y));
}

void GameScreen::restartCountdown() {
  countdownTimer.start();
}

void GameScreen::handleGameOver() {
  if (gameOver)
    return;

  gameOver = true;

  if (game.isNewHighScore()) {
    game.saveScoreToRecordTable();
  }

  game.setCurrentScreen(new HighScores(window, game));
}

void GameScreen::initializeGrid() {
  const float scaleRelativeFactor = gameGrid.getScaleFactor();
  const float scale = getScale(sf::Vector2f(gridSize, gridSize), window.getSize()) * scaleRelativeFactor;

  const auto position = getPosition(sf::Vector2f(gridSize, gridSize), window.getSize(), scale);

  gameGrid.updateGrid(position, scale);
}

void GameScreen::updateGrid() {
  const float scaleRelativeFactor = gameGrid.getScaleFactor();
  const float scale = getScale(sf::Vector2f(gridSize, gridSize), window.getSize()) * scaleRelativeFactor;
  const auto position = getPosition(sf::Vector2f(gridSize, gridSize), window.getSize(), scale);

  gameGrid.updateGrid(position, scale);
}

void GameScreen::resume() {
  updateGrid();

  countdownTimer.start();

  resumeMusic();

  unpause();
}

void GameScreen::pauseMusic() {
  if (backgroundMusic && musicStarted) {
    backgroundMusic->stop();
    musicStarted = false;
  }
}

void GameScreen::resumeMusic() {
  if (backgroundMusic && musicEnabled && !musicStarted) {
    backgroundMusic->play();
    musicStarted = true;
  }
}

void GameScreen::pause() {
  isPaused = true;

  TimerManager::getInstance().pauseAll();
}

void GameScreen::unpause() {
  isPaused = false;

  TimerManager::getInstance().unpauseAll();
}

void GameScreen::startBlinking() {
  isBlinking = true;
  blinkCount = 0;
  blinkTimer.restart();
}
