#include "GameScreen.hpp"
#include <iostream>
#include "../utils/ResourceLoader.hpp"
#include "PauseScreen.hpp"

GameScreen::GameScreen(sf::RenderWindow& win, Game& gameRef)
    : Screen(win, gameRef), gameGrid(32, 32, 824.0f, sf::Vector2f(0, 0), 1.0f, 912.0f) {
  initializeGrid();
}

void GameScreen::processEvents(const sf::Event& event) {

  if (event.is<sf::Event::KeyPressed>()) {
    switch (event.getIf<sf::Event::KeyPressed>()->code) {
      case sf::Keyboard::Key::Escape:
      case sf::Keyboard::Key::P:
        game.setCurrentScreen(new PauseScreen(window, game));
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
  // std::cout << "GameScreen screen - Updating game logic" << std::endl;
}

sf::Vector2f GameScreen::getPosition(const sf::Vector2f size, const float scale) const {
  const sf::Vector2u windowSize = window.getSize();

  const float positionX = (windowSize.x - size.x * scale) / 2.0f;
  const float positionY = (windowSize.y - size.y * scale) / 2.0f;

  const auto position = sf::Vector2f(positionX, positionY);

  return position;
}

void GameScreen::renderBoard() {}

void GameScreen::renderBoardBorder() const {
  const auto texture = utils::ResourceLoader::getTexture(utils::TextureType::BoardBorder);
  sf::Sprite sprite(texture);

  const float scale = getScale(sf::Vector2f(sprite.getTexture().getSize()));

  sprite.setScale(sf::Vector2f(scale, scale));

  const auto position = getPosition(sf::Vector2f(sprite.getTexture().getSize()), scale);

  sprite.setPosition(position);

  // sprite.setColor(sf::Color(255, 255, 255, 48));

  window.draw(sprite);
}

void GameScreen::renderBoardGrid() const {
  const auto texture = utils::ResourceLoader::getTexture(utils::TextureType::BoardGrid);
  sf::Sprite sprite(texture);

  const float scaleRelativeFactor = 912.0f / 992.0f;
  const float scale = getScale(sf::Vector2f(sprite.getTexture().getSize())) * scaleRelativeFactor;

  sprite.setScale(sf::Vector2f(scale, scale));
  const auto position = getPosition(sf::Vector2f(sprite.getTexture().getSize()), scale);

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

  renderDebugGrid();
}

void GameScreen::initializeGrid() {
  const float gridSize = 824.0f;
  const float scaleRelativeFactor = gameGrid.getScaleFactor();
  const float scale = getScale(sf::Vector2f(gridSize, gridSize)) * scaleRelativeFactor;

  const auto position = getPosition(sf::Vector2f(gridSize, gridSize), scale);

  // Update the game grid with new position and scale
  gameGrid.updateGrid(position, scale);
}
