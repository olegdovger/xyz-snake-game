#include <algorithm>
#include <iostream>
#include "../Game.hpp"
#include "../Screen.hpp"

#include "Difficulty.cpp"
#include "Exit.cpp"
#include "GameScreen.cpp"
#include "HighScores.cpp"
#include "Settings.cpp"

class MainMenu : public Screen {
public:
  explicit MainMenu(sf::RenderWindow& win, Game& gameRef)
      : Screen(win, gameRef) {}

  void processEvents(const sf::Event& event) override {
    std::cout << "MainMenu screen - Processing events" << std::endl;
    // Access game state
    std::cout << "Current score: " << game.getScore() << std::endl;
    std::cout << "High score: " << game.getHighScore() << std::endl;

    if (event.is<sf::Event::KeyPressed>()) {
      switch (event.getIf<sf::Event::KeyPressed>()->code) {
        case sf::Keyboard::Key::Num1:
          game.setCurrentScreen(new GameScreen(window, game));
          break;
        case sf::Keyboard::Key::Num2:
          game.setCurrentScreen(new Difficulty(window, game));
          break;
        case sf::Keyboard::Key::Num3:
          game.setCurrentScreen(new HighScores(window, game));
          break;
        case sf::Keyboard::Key::Num4:
          game.setCurrentScreen(new Settings(window, game));
          break;
        case sf::Keyboard::Key::Escape:
          game.setCurrentScreen(new Exit(window, game));
          break;
        default:
          break;
      }
    }
  }

  void update() override {
    std::cout << "MainMenu screen - Updating" << std::endl;
  }

  void render() override { std::cout << "Drawing Main Menu" << std::endl; }
};
