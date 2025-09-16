#include "Difficulty.hpp"
#include <iostream>

Difficulty::Difficulty(sf::RenderWindow& win, Game& gameRef) : Screen(win, gameRef) {}

void Difficulty::processEvents(const sf::Event& event) {
  std::cout << "Difficulty screen - Handling difficulty input" << std::endl;
}

void Difficulty::update() {
  std::cout << "Difficulty screen - Updating difficulty selection" << std::endl;
}

void Difficulty::render() {
  std::cout << "Difficulty screen - Select difficulty level" << std::endl;
}
