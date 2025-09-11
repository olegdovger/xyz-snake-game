#include <iostream>
#include "../Screen.hpp"

class Exit : public Screen {
 public:
  explicit Exit(sf::RenderWindow& win, Game& gameRef) : Screen(win, gameRef) {}

  void processEvents(const sf::Event& event) override {
    std::cout << "Exit screen - Handling exit events" << std::endl;
    // Close the window when exit screen is reached
    window.close();
  }

  void update() override { std::cout << "Exit screen - Updating" << std::endl; }

  void render() override { std::cout << "Exit screen - Goodbye!" << std::endl; }
};
