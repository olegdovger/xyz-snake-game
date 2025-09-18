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

  void renderBoardBorder() const;

  void renderBoardGrid() const;

  void renderDebugGrid() const;

  void initializeGrid();
};
