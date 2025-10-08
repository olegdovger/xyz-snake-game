#include "GameScreen.hpp"
#include "../utils/GameItem.hpp"
#include "../utils/GameUI.hpp"
#include "../utils/ResourceLoader.hpp"
#include "../utils/ScalingUtils.hpp"
#include "../utils/SettingStorage.hpp"
#include "HighScores.hpp"
#include "PauseScreen.hpp"

using namespace shape;

GameScreen::GameScreen(sf::RenderWindow& win, Game& gameRef)
    : Screen(win, gameRef),
      gameGrid(32, 32, 824.0f, sf::Vector2f(0, 0), 1.0f, 912.0f),
      snake(sf::Vector2i(16, 16), 5),
      countdownTimer(1, false),
      currentSnakeTypeIndex(0) {
  initializeGrid();

  SettingStorage settingStorage;

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

  gameUI = GameUI();

  // Reset game score and GameUI score
  game.resetScore();
  gameUI.setScore(0);
  gameUI.setApples(0);

  // Initialize wall manager
  wallManager = std::make_unique<WallManager>(gameGrid);

  // Initialize game item manager
  gameItemManager = std::make_unique<GameItemManager>(gameGrid);
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
  // Update walls
  if (wallManager) {
    wallManager->update(1.0f / 60.0f, snake);  // Assuming 60 FPS
  }

  // Update game items
  if (gameItemManager) {
    gameItemManager->update(1.0f / 60.0f, snake);  // Assuming 60 FPS
  }

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

      // Check game item collisions
      if (gameItemManager) {
        auto* collidedItem = gameItemManager->checkCollision(snake.getHead());
        if (collidedItem) {
          // Apply item effects
          collidedItem->applySpecialEffects(snake);

          // Make snake grow when eating any item
          snake.grow();

          game.addScore(collidedItem->getPoints());
          gameUI.setScore(game.getScore());
          gameUI.setApples(gameUI.getApples() + 1);

          // Remove the item immediately
          gameItemManager->removeItem(collidedItem);
        }
      }

      // Check collisions with walls, grid boundaries, and self
      bool wallCollision = false;
      if (wallManager) {
        wallCollision = wallManager->checkWallCollision(snake.getHead());
      }

      if (!snake.isInvincible()) {
        if (wallCollision || snake.checkWallCollision(gameGrid.getCols(), gameGrid.getRows()) ||
            snake.checkSelfCollision()) {
          snake.kill();
          startBlinking();
        }
      }

      moveTimer.restart();
    }
  }

  // Update countdown timer
  countdownTimer.update();
}

sf::Sprite GameScreen::renderBoardBorder() const {
  const auto texture = ResourceLoader::getTexture(TextureType::BoardBorder);
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
  const auto texture = ResourceLoader::getTexture(TextureType::BoardGrid);
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

  renderGameUI(boardBorder);

  if (wallManager) {
    wallManager->render(window, gameGrid);
  }

  if (gameItemManager) {
    gameItemManager->render(window, gameGrid);
  }

  snake.setBlinking(isBlinking);
  if (isBlinking) {
    snake.setBlinkTimer(blinkTimer);
    std::cout << "Setting snake blinking state: isBlinking=" << isBlinking << std::endl;
  }

  snake.render(window, gameGrid);

  // Render countdown timer last to be on top of everything
  if (countdownTimer.getIsActive()) {
    // Center the countdown text on screen
    sf::Vector2u windowSize = window.getSize();
    countdownTimer.setPosition(sf::Vector2f(windowSize.x / 2.0f, windowSize.y / 2.0f));
    countdownTimer.render(window);
  }
}

void GameScreen::renderGameUI(const sf::Sprite& boardBorder) const {
  // scale it
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
  if (!gameOver) {
    gameOver = true;

    // Score is already tracked properly through item collection
    int finalScore = game.getScore();

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
