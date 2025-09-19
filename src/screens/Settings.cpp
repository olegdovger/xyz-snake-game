#include "Settings.hpp"
#include <iostream>

Settings::Settings(sf::RenderWindow& win, Game& gameRef) : Screen(win, gameRef) {}

void Settings::processEvents(const sf::Event& event) {
  std::cout << "Settings screen - Handling settings input" << std::endl;
}

void Settings::update() {
  std::cout << "Settings screen - Updating settings display" << std::endl;
}

void Settings::render() {
  std::cout << "Settings screen - Game configuration options" << std::endl;
}
