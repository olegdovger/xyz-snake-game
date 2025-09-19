#pragma once

#include "../Screen.hpp"

class Settings : public Screen {
public:
  explicit Settings(sf::RenderWindow& win, Game& gameRef);

  void processEvents(const sf::Event& event) override;
  void update() override;
  void render() override;
};
