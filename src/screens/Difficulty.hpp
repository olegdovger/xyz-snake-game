#pragma once

#include "../Screen.hpp"

class Difficulty : public Screen {
public:
  explicit Difficulty(sf::RenderWindow& win, Game& gameRef);

  void processEvents(const sf::Event& event) override;
  void update() override;
  void render() override;
};
