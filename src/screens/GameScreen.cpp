#include "GameScreen.hpp"
#include "../utils/GameUI.hpp"
#include "../utils/ResourceLoader.hpp"
#include "../utils/ScalingUtils.hpp"
#include "../utils/SettingStorage.hpp"
#include "HighScores.hpp"
#include "PauseScreen.hpp"

using namespace utils::shape;

GameScreen::GameScreen(sf::RenderWindow& win, Game& gameRef)
    : Screen(win, gameRef),
      gameGrid(32, 32, 824.0f, sf::Vector2f(0, 0), 1.0f, 912.0f),
      snake(sf::Vector2i(16, 16), 5),
      countdownTimer(1, false),
      currentSnakeTypeIndex(0) {
  initializeGrid();

  utils::SettingStorage settingStorage;

  if (settingStorage.initialize()) {
    snake.setSnakeType(settingStorage.getSnakeType());
    game.setGameLevel(settingStorage.getGameLevel());

    // Configure countdown timer based on settings
    if (settingStorage.getGameCountdownEnabled()) {
      countdownTimer.setDuration(settingStorage.getGameCountdownInSeconds());
      countdownTimer.start();
    }
  }

  // Initialize snake type timer for testing
  snakeTypeTimer.restart();

  snake.setSpeed(settingStorage.getSnakeSpeed());

  gameUI = utils::GameUI();
}

void GameScreen::processEvents(const sf::Event& event) {

  if (event.is<sf::Event::KeyPressed>()) {
    switch (event.getIf<sf::Event::KeyPressed>()->code) {
      case sf::Keyboard::Key::Escape:
        // Pause game
        game.setCurrentScreenWithPrevious(new PauseScreen(window, game), this);
        break;
      case sf::Keyboard::Key::P:
        if (!gameOver) {
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
  // Update countdown timer
  countdownTimer.update();

  // Handle blinking effect
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

  // Only update game logic if countdown is finished and not blinking
  if (countdownTimer.getIsFinished() && !isBlinking) {
    // Move snake every step
    if (moveTimer.getElapsedTime().asSeconds() >= 1 / snake.getSpeed()) {
      snake.move();

      if (snake.checkWallCollision(gameGrid.getCols(), gameGrid.getRows()) || snake.checkSelfCollision()) {
        snake.kill();
        startBlinking();
      }

      moveTimer.restart();
    }

    // Change snake type every second for testing
    if (snakeTypeTimer.getElapsedTime().asSeconds() >= SNAKE_TYPE_CHANGE_INTERVAL) {
      currentSnakeTypeIndex = (currentSnakeTypeIndex + 1) % TOTAL_SNAKE_TYPES;

      // Convert index to SnakeType enum
      utils::SnakeSprite::SnakeType newType = static_cast<utils::SnakeSprite::SnakeType>(currentSnakeTypeIndex);
      snake.setSnakeType(newType);

      snakeTypeTimer.restart();
    }
  }
}

sf::Sprite GameScreen::renderBoardBorder() const {
  const auto texture = utils::ResourceLoader::getTexture(utils::TextureType::BoardBorder);
  sf::Sprite sprite(texture);

  const float scale = getScale(sf::Vector2f(sprite.getTexture().getSize()), window.getSize());

  sprite.setScale(sf::Vector2f(scale, scale));

  const auto position = getPosition(sf::Vector2f(sprite.getTexture().getSize()), window.getSize(), scale);

  sprite.setPosition(position);

  // sprite.setColor(sf::Color(255, 255, 255, 48));

  window.draw(sprite);

  return sprite;
}

void GameScreen::renderBoardGrid() const {
  const auto texture = utils::ResourceLoader::getTexture(utils::TextureType::BoardGrid);
  sf::Sprite sprite(texture);

  const float scaleRelativeFactor = 912.0f / 992.0f;
  const float scale = getScale(sf::Vector2f(sprite.getTexture().getSize()), window.getSize()) * scaleRelativeFactor;

  sprite.setScale(sf::Vector2f(scale, scale));
  const auto position = getPosition(sf::Vector2f(sprite.getTexture().getSize()), window.getSize(), scale);

  sprite.setPosition(position);

  // sprite.setColor(sf::Color(255, 255, 255, 128));

  window.draw(sprite);
}

void GameScreen::renderDebugGrid() const {
  // Draw border using GameGrid data
  sf::RectangleShape border(sf::Vector2f(gameGrid.getGridBounds().size.x, gameGrid.getGridBounds().size.y));
  border.setFillColor(sf::Color::Transparent);
  border.setOutlineColor(sf::Color(255, 255, 255, 128));
  border.setOutlineThickness(8.0f * gameGrid.getScale());

  border.setPosition(gameGrid.getTopLeft());

  window.draw(border);

  // Draw cells using GameGrid data
  sf::RectangleShape cell(sf::Vector2f(gameGrid.getCellSize(), gameGrid.getCellSize()));
  cell.setScale(sf::Vector2f(gameGrid.getScale(), gameGrid.getScale()));
  cell.setFillColor(sf::Color::Transparent);
  cell.setOutlineColor(sf::Color(0, 255, 0, 128));
  cell.setOutlineThickness(1.0f * gameGrid.getScale());

  // Render all cells using GameGrid
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

  // Render countdown timer if active
  if (countdownTimer.getIsActive()) {
    // Center the countdown text on screen
    sf::Vector2u windowSize = window.getSize();
    countdownTimer.setPosition(sf::Vector2f(windowSize.x / 2.0f, windowSize.y / 2.0f));
    countdownTimer.render(window);
    std::cout << "Rendering countdown: " << countdownTimer.getRemainingSeconds() << std::endl;
  }

  // renderDebugGrid();
  renderGameUI(boardBorder);

  // Set blinking state and timer for snake
  snake.setBlinking(isBlinking);
  if (isBlinking) {
    snake.setBlinkTimer(blinkTimer);
    std::cout << "Setting snake blinking state: isBlinking=" << isBlinking << std::endl;
  }

  // Render snake last (highest z-index) - always on top of other elements
  snake.render(window, gameGrid);
}

void GameScreen::renderGameUI(const sf::Sprite& boardBorder) const {
  // scale it
  const auto boardBorderSize = boardBorder.getLocalBounds().size;
  const auto position =
      getPosition(sf::Vector2f(boardBorder.getTexture().getSize()), window.getSize(), boardBorder.getScale().x);

  gameUI.setScale(boardBorder.getScale().x);
  gameUI.render(
      window,
      sf::Vector2f(boardBorder.getGlobalBounds().position.x + (boardBorderSize.x + 16) * boardBorder.getScale().x,
                   boardBorder.getGlobalBounds().position.y),
      1234567890, 1234567890, 1234567890);
}

void GameScreen::restartCountdown() {
  countdownTimer.start();
}

void GameScreen::handleGameOver() {
  if (!gameOver) {
    gameOver = true;

    // Calculate score based on snake length (you can modify this logic)
    int finalScore = snake.getLength() * 10;
    game.setScore(finalScore);

    // Save score to record table if it qualifies
    if (game.isNewHighScore()) {
      game.saveScoreToRecordTable();
      std::cout << "New high score! Score: " << finalScore << std::endl;
    } else {
      std::cout << "Game over! Score: " << finalScore << std::endl;
    }

    // Switch to HighScores screen to show the updated record table
    game.setCurrentScreen(new HighScores(window, game));
  }
}

void GameScreen::initializeGrid() {
  const float scaleRelativeFactor = gameGrid.getScaleFactor();
  const float scale = getScale(sf::Vector2f(gridSize, gridSize), window.getSize()) * scaleRelativeFactor;

  const auto position = getPosition(sf::Vector2f(gridSize, gridSize), window.getSize(), scale);

  // Update the game grid with new position and scale
  gameGrid.updateGrid(position, scale);
}

void GameScreen::updateGrid() {
  const float scaleRelativeFactor = gameGrid.getScaleFactor();
  const float scale = getScale(sf::Vector2f(gridSize, gridSize), window.getSize()) * scaleRelativeFactor;
  const auto position = getPosition(sf::Vector2f(gridSize, gridSize), window.getSize(), scale);

  // Update the game grid with new position and scale
  gameGrid.updateGrid(position, scale);
}

void GameScreen::resume() {
  updateGrid();

  countdownTimer.start();
}

void GameScreen::startBlinking() {
  isBlinking = true;
  blinkCount = 0;
  blinkTimer.restart();
}
