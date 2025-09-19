#include "GameScreen.hpp"
#include "../utils/ResourceLoader.hpp"
#include "../utils/ScalingUtils.hpp"
#include "PauseScreen.hpp"

using namespace utils::shape;

GameScreen::GameScreen(sf::RenderWindow& win, Game& gameRef)
    : Screen(win, gameRef), gameGrid(32, 32, 824.0f, sf::Vector2f(0, 0), 1.0f, 912.0f), snake(sf::Vector2i(16, 16), 5) {
  initializeGrid();
  snake.setSnakeType(utils::SnakeSprite::SnakeType::Purple);
}

void GameScreen::processEvents(const sf::Event& event) {

  if (event.is<sf::Event::KeyPressed>()) {
    switch (event.getIf<sf::Event::KeyPressed>()->code) {
      case sf::Keyboard::Key::Escape:
      case sf::Keyboard::Key::P:
        game.setCurrentScreen(new PauseScreen(window, game));
        break;
      case sf::Keyboard::Key::Up:
        snake.setDirection(Snake::Direction::Up);
        break;
      case sf::Keyboard::Key::Down:
        snake.setDirection(Snake::Direction::Down);
        break;
      case sf::Keyboard::Key::Left:
        snake.setDirection(Snake::Direction::Left);
        break;
      case sf::Keyboard::Key::Right:
        snake.setDirection(Snake::Direction::Right);
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
  // Move snake every second
  if (moveTimer.getElapsedTime().asSeconds() >= SNAKE_MOVE_INTERVAL) {
    snake.move();
    moveTimer.restart();

    // Check collisions
    if (snake.checkSelfCollision() || snake.checkWallCollision(gameGrid.getCols(), gameGrid.getRows())) {
      snake.kill();
    }
  }
}

void GameScreen::renderBoardBorder() const {
  const auto texture = utils::ResourceLoader::getTexture(utils::TextureType::BoardBorder);
  sf::Sprite sprite(texture);

  const float scale = getScale(sf::Vector2f(sprite.getTexture().getSize()), window.getSize());

  sprite.setScale(sf::Vector2f(scale, scale));

  const auto position = getPosition(sf::Vector2f(sprite.getTexture().getSize()), window.getSize(), scale);

  sprite.setPosition(position);

  // sprite.setColor(sf::Color(255, 255, 255, 48));

  window.draw(sprite);
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
  renderBoardBorder();
  renderBoardGrid();

  // Render snake
  snake.render(window, gameGrid);

  // renderDebugGrid();
}

void GameScreen::initializeGrid() {
  const float gridSize = 824.0f;
  const float scaleRelativeFactor = gameGrid.getScaleFactor();
  const float scale = getScale(sf::Vector2f(gridSize, gridSize), window.getSize()) * scaleRelativeFactor;

  const auto position = getPosition(sf::Vector2f(gridSize, gridSize), window.getSize(), scale);

  // Update the game grid with new position and scale
  gameGrid.updateGrid(position, scale);
}
