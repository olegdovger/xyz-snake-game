#include <iostream>
#include "../Screen.hpp"

class Settings : public Screen {
 public:
  explicit Settings(sf::RenderWindow& win, Game& gameRef)
      : Screen(win, gameRef) {}

  void processEvents(const sf::Event& event) override {
    std::cout << "Settings screen - Handling settings input" << std::endl;
  }

  void update() override {
    std::cout << "Settings screen - Updating settings display" << std::endl;
  }

  void render() override {
    std::cout << "Settings screen - Game configuration options" << std::endl;
  }
};
