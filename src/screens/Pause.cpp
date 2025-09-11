#include <iostream>
#include "../Screen.hpp"

class Pause : public Screen {
 public:
  explicit Pause(sf::RenderWindow& win, Game& gameRef) : Screen(win, gameRef) {}

  void processEvents(const sf::Event& event) override {
    std::cout << "Pause screen - Handling pause menu input" << std::endl;
  }

  void update() override {
    std::cout << "Pause screen - Updating pause menu" << std::endl;
  }

  void render() override {
    std::cout << "Pause screen - Game paused" << std::endl;
  }
};
