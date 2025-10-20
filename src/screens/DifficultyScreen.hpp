#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "../Game.hpp"
#include "../Screen.hpp"

class DifficultyScreen : public Screen {
public:
  explicit DifficultyScreen(sf::RenderWindow& win, Game& gameRef);

  void processEvents(const sf::Event& event) override;
  void update() override;
  void render() override;

private:
  sf::RectangleShape screenRect;
  sf::Font font;
  sf::Text titleText;

  sf::Text backText;
  std::vector<sf::Text> difficultyItems;
  int selectedDifficultyIndex = 0;
  sf::Vector2f originSize = sf::Vector2f(600.0f, 600.0f);

  sf::Color backgroundColor = sf::Color(164, 144, 164);

  sf::Color menuBackgroundColor = MenuColors::BACKGROUND_COLOR;
  sf::Color textColor = MenuColors::TEXT_COLOR;
  sf::Color borderColor = MenuColors::BORDER_COLOR;

  void renderScreenRect();
  void renderScreenItems();
  void renderTitle();

  void renderBackButton();
  void initializeDifficultyItems();
  void selectNextDifficulty();
  void selectPreviousDifficulty();
  void confirmSelection();

  // Menu sounds
  sf::Sound setActiveMenuItemSound;
  sf::Sound selectMenuItemSound;
};
