#include <iostream>
#include "../Screen.hpp"

class HighScores : public Screen {
 public:
  explicit HighScores(sf::RenderWindow& win, Game& gameRef)
      : Screen(win, gameRef) {}

  void processEvents(const sf::Event& event) override {
    std::cout << "HighScores screen - Handling score navigation" << std::endl;
  }

  void update() override {
    std::cout << "HighScores screen - Updating score display" << std::endl;
  }

  void render() override {
    std::cout << "HighScores screen - Displaying top scores" << std::endl;
  }
};
