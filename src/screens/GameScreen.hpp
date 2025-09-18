#pragma once
#include "../Screen.hpp"
#include "../utils/GameGrid.hpp"

class GameScreen final : public Screen {
public:
  explicit GameScreen(sf::RenderWindow& win, Game& gameRef);

  void processEvents(const sf::Event& event) override;

  void update() override;

  void render() override;

private:
  // Grid data
  utils::GameGrid gameGrid;

  float getScale(const sf::Vector2f size) const {
    const auto windowSize = sf::Vector2f(window.getSize());
    const auto objectSize = size;
    const float scaleX = (float)(windowSize.x * 0.8f) / objectSize.x;
    const float scaleY = (float)(windowSize.y * 0.8f) / objectSize.y;
    const float scale = std::min(scaleX, scaleY);

    return scale;
  }

  sf::Vector2f getPosition(sf::Vector2f size, float scale) const;

  static void renderBoard();

  void renderBoardBorder() const;

  void renderBoardGrid() const;

  void renderDebugGrid() const;

  void initializeGrid();
};
