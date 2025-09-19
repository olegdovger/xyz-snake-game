#pragma once
#include <SFML/System/Clock.hpp>
#include "../Screen.hpp"
#include "../Snake.hpp"
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

  // Snake
  Snake snake;

  // Timing
  sf::Clock moveTimer;
  static constexpr float MOVE_INTERVAL = 1.0f;  // 1 second

  void renderBoardBorder() const;

  void renderBoardGrid() const;

  void renderDebugGrid() const;

  void initializeGrid();
};
