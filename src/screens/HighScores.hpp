#pragma once

#include "../Screen.hpp"

class HighScores : public Screen {
public:
  explicit HighScores(sf::RenderWindow& win, Game& gameRef);

  void processEvents(const sf::Event& event) override;
  void update() override;
  void render() override;
};
