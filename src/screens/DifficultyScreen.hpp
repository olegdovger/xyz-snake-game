#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
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

  static constexpr uint8_t TEXT_R = 251;  // 251, 242, 54
  static constexpr uint8_t TEXT_G = 242;  // 251, 242, 54
  static constexpr uint8_t TEXT_B = 54;   // 251, 242, 54

  sf::Color textColor = sf::Color(TEXT_R, TEXT_G, TEXT_B);

  static constexpr uint8_t BACKGROUND_R = 130;
  static constexpr uint8_t BACKGROUND_G = 73;
  static constexpr uint8_t BACKGROUND_B = 113;

  sf::Color menuBackgroundColor = sf::Color(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B);

  static constexpr uint8_t BORDER_R = 171;
  static constexpr uint8_t BORDER_G = 99;
  static constexpr uint8_t BORDER_B = 148;

  sf::Color borderColor = sf::Color(BORDER_R, BORDER_G, BORDER_B);

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
