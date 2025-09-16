#include "HighScores.hpp"
#include <iostream>

HighScores::HighScores(sf::RenderWindow& win, Game& gameRef) : Screen(win, gameRef) {}

void HighScores::processEvents(const sf::Event& event) {
  std::cout << "HighScores screen - Handling score navigation" << std::endl;
}

void HighScores::update() {
  std::cout << "HighScores screen - Updating score display" << std::endl;
}

void HighScores::render() {
  std::cout << "HighScores screen - Displaying top scores" << std::endl;
}
