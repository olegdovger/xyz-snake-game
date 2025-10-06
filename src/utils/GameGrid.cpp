#include "GameGrid.hpp"
#include <SFML/Graphics.hpp>

GameGrid::GameGrid(int rows, int cols, float gridSize, sf::Vector2f topLeft, float cellScale, float gridTextureSize)
    : rows(rows), cols(cols), topLeft(topLeft), scale(cellScale), gridTextureSize(gridTextureSize) {
  scaleFactor = gridSize / gridTextureSize;
  cellSize = gridSize / cols;
  scaledCellSize = cellSize * cellScale;
  calculateCellPositions();
}

void GameGrid::calculateCellPositions() {
  cellPositions.resize(rows);
  for (int row = 0; row < rows; ++row) {
    cellPositions[row].resize(cols);
    for (int col = 0; col < cols; ++col) {
      const float x = topLeft.x + col * scaledCellSize;
      const float y = topLeft.y + row * scaledCellSize;
      cellPositions[row][col] = sf::Vector2f(x, y);
    }
  }
}

sf::Vector2f GameGrid::getCellPosition(int row, int col) const {
  if (!isValidPosition(row, col)) {
    const float x = topLeft.x + col * scaledCellSize;
    const float y = topLeft.y + row * scaledCellSize;
    return sf::Vector2f(x, y);
  }
  return cellPositions[row][col];
}

sf::Vector2f GameGrid::getCellCenter(int row, int col) const {
  if (!isValidPosition(row, col)) {
    return sf::Vector2f(0, 0);
  }
  const sf::Vector2f position = cellPositions[row][col];
  return sf::Vector2f(position.x + scaledCellSize / 2.0f, position.y + scaledCellSize / 2.0f);
}

bool GameGrid::isValidPosition(int row, int col) const {
  return row >= 0 && row < rows && col >= 0 && col < cols;
}

sf::FloatRect GameGrid::getGridBounds() const {
  return sf::FloatRect(topLeft, sf::Vector2f(cols * scaledCellSize, rows * scaledCellSize));
}

sf::FloatRect GameGrid::getCellBounds(int row, int col) const {
  if (!isValidPosition(row, col)) {
    return sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(0, 0));
  }
  const sf::Vector2f position = cellPositions[row][col];
  return sf::FloatRect(position, sf::Vector2f(scaledCellSize, scaledCellSize));
}

void GameGrid::updateGrid(sf::Vector2f newTopLeft, float newCellScale) {
  topLeft = newTopLeft;
  scale = newCellScale;
  scaledCellSize = cellSize * newCellScale;
  calculateCellPositions();
}
