#pragma once

#include <iostream>
#include "../Screen.hpp"
#include "../utils/ResourceLoader.hpp"

class GameScreen final : public Screen {
public:
  explicit GameScreen(sf::RenderWindow& win, Game& gameRef);

  void processEvents(const sf::Event& event) override;
  void update() override;
  void render() override;

private:
  float rescaleSprite(sf::Sprite& sprite, float scaleRelativeFactor = 1.0f) const;
  void setSpritePositionToCenter(sf::Sprite& sprite, float scale) const;
  void renderBoard();
  void renderBoardBorder();
  void renderBoardGrid();
};
