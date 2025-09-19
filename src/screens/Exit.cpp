#include "Exit.hpp"
#include <iostream>

Exit::Exit(sf::RenderWindow& win, Game& gameRef) : Screen(win, gameRef) {}

void Exit::processEvents(const sf::Event& event) {
  std::cout << "Exit screen - Handling exit events" << std::endl;
  // Close the window when exit screen is reached
  window.close();
}

void Exit::update() {
  std::cout << "Exit screen - Updating" << std::endl;
}

void Exit::render() {
  std::cout << "Exit screen - Goodbye!" << std::endl;
}
