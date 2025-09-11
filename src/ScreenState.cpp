#include "ScreenState.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

ScreenState::ScreenState(sf::RenderWindow& win)
    : selectedScreen(ScreenEnum::MainMenu), window(win) {}

void ScreenState::setSelectedScreen(ScreenEnum state) {
  selectedScreen = state;

  // Output current state using cout
  std::string screenName;
  switch (selectedScreen) {
    case ScreenEnum::MainMenu:
      screenName = "Main Menu";
      break;
    case ScreenEnum::GameScreen:
      screenName = "Start Game";
      break;
    case ScreenEnum::Difficulty:
      screenName = "Difficulty Selection";
      break;
    case ScreenEnum::HighScores:
      screenName = "High Scores";
      break;
    case ScreenEnum::Settings:
      screenName = "Settings";
      break;
    case ScreenEnum::Pause:
      screenName = "Pause Menu";
      break;
    case ScreenEnum::Exit:
      screenName = "Exit";
      break;
  }

  std::cout << "Screen changed to: " << screenName << std::endl;
}

ScreenEnum ScreenState::getSelectedScreen() const {
  return selectedScreen;
}
