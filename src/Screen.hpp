#pragma once
#include <SFML/Graphics.hpp>

class Game;

class Screen {
protected:
  sf::RenderWindow& window;

public:
  explicit Screen(sf::RenderWindow& win, Game& gameRef);

  virtual ~Screen() = default;

  Game& game;

  virtual void processEvents(const sf::Event& event) = 0;

  virtual void update() = 0;

  virtual void render() = 0;
};
