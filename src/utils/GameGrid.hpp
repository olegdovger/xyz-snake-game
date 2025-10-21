#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>

class GameGrid {
public:
  GameGrid(int rows, int cols, float gridSize, sf::Vector2f topLeft, float cellScale, float gridTextureSize = 912.0f);

  int getRows() const { return rows; }
  int getCols() const { return cols; }
  float getCellSize() const { return cellSize; }
  float getScaledCellSize() const { return scaledCellSize; }
  sf::Vector2f getTopLeft() const { return topLeft; }
  float getScale() const { return scale; }

  sf::Vector2f getCellPosition(int row, int col) const;
  sf::Vector2f getCellCenter(int row, int col) const;
  bool isValidPosition(int row, int col) const;

  sf::FloatRect getGridBounds() const;
  sf::FloatRect getCellBounds(int row, int col) const;

  void updateGrid(sf::Vector2f newTopLeft, float newCellScale);

  float getScaleFactor() const { return scaleFactor; }

private:
  int rows;
  int cols;
  float cellSize;
  float scaledCellSize;
  sf::Vector2f topLeft;
  float scale;
  float scaleFactor;
  float gridTextureSize;
  std::vector<std::vector<sf::Vector2f>> cellPositions;

  void calculateCellPositions();
};
