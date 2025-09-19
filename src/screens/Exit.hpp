#pragma once

#include "../Screen.hpp"

class Exit : public Screen {
public:
  explicit Exit(sf::RenderWindow& win, Game& gameRef);

  void processEvents(const sf::Event& event) override;
  void update() override;
  void render() override;
};
