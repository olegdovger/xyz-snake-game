#pragma once
#include <SFML/Graphics.hpp>

enum class ScreenEnum {
  MainMenu,
  GameScreen,
  Difficulty,
  HighScores,
  Settings,
  Pause,
  Exit
};

struct ScreenState {
  ScreenEnum selectedScreen;
  sf::RenderWindow& window;

  explicit ScreenState(sf::RenderWindow& win);
  ~ScreenState() = default;

  void setSelectedScreen(ScreenEnum screen);
  [[nodiscard]] ScreenEnum getSelectedScreen() const;
};
