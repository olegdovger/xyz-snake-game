#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "../Screen.hpp"

class HighScores : public Screen {
private:
  sf::Font font;
  sf::Text titleText;
  std::vector<sf::Text> scoreTexts;
  sf::Text backText;

  // Colors
  sf::Color backgroundColor = sf::Color(164, 144, 164);
  sf::Color menuBackgroundColor = sf::Color(130, 73, 113);
  sf::Color textColor = sf::Color(251, 242, 54);
  sf::Color borderColor = sf::Color(171, 99, 148);

  // Menu rectangle
  sf::RectangleShape menuRect;
  sf::Vector2f menuRectSize = sf::Vector2f(600.0f, 500.0f);

  void renderMenuRect();
  void renderTitle();
  void renderScores();
  void renderBackButton();

public:
  explicit HighScores(sf::RenderWindow& win, Game& gameRef);

  void processEvents(const sf::Event& event) override;
  void update() override;
  void render() override;
};
