#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <vector>
#include "../Game.hpp"
#include "../Screen.hpp"

class PauseScreen final : public Screen {
private:
  sf::RectangleShape screenRect;
  sf::Font font;
  sf::Text titleText;
  std::vector<sf::Text> menuItems;
  int selectedIndex = 0;
  sf::Vector2f originSize = sf::Vector2f(400.0f, 400.0f);

  static constexpr size_t MENU_ITEMS_COUNT = 3;

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

  float scaleRelativeFactor = 0.6f;

  sf::Vector2f menuRectSize = originSize * scaleRelativeFactor;

  // Menu sounds
  sf::Sound setActiveMenuItemSound;
  sf::Sound selectMenuItemSound;

  void initializeMenuItems();
  void renderMenuRect();
  void renderTitle();
  void renderMenuItems();

public:
  explicit PauseScreen(sf::RenderWindow& win, Game& gameRef);

  void processEvents(const sf::Event& event) override;
  void update() override;
  void render() override;
};
