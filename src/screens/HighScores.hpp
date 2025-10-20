#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "../Game.hpp"
#include "../Screen.hpp"

class HighScores : public Screen {
private:
  sf::RectangleShape screenRect;
  sf::Font font;
  sf::Text titleText;
  std::vector<sf::Text> scoreTexts;
  sf::Text backText;
  sf::Vector2f originSize = sf::Vector2f(600.0f, 500.0f);

  static constexpr float SCORES_COUNT = 5;

  // Colors
  sf::Color backgroundColor = sf::Color(164, 144, 164);
  sf::Color menuBackgroundColor = MenuColors::BACKGROUND_COLOR;
  sf::Color textColor = MenuColors::TEXT_COLOR;
  sf::Color borderColor = MenuColors::BORDER_COLOR;

  void initializeScreenRect();

  void renderScreenRect();
  void renderTitle();
  void renderScores();
  void renderBackButton();

public:
  explicit HighScores(sf::RenderWindow& win, Game& gameRef);

  void processEvents(const sf::Event& event) override;
  void update() override;
  void render() override;
};
