#include <iostream>
#include "../Screen.hpp"

class Difficulty : public Screen {
 public:
  explicit Difficulty(sf::RenderWindow& win, Game& gameRef)
      : Screen(win, gameRef) {}

  void processEvents(const sf::Event& event) override {
    std::cout << "Difficulty screen - Handling difficulty input" << std::endl;
  }

  void update() override {
    std::cout << "Difficulty screen - Updating difficulty selection"
              << std::endl;
  }

  void render() override {
    std::cout << "Difficulty screen - Select difficulty level" << std::endl;
  }
};
