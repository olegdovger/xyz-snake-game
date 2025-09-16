#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <string>
#include <vector>
#include "../Game.hpp"
#include "../Screen.hpp"

class MainMenu : public Screen {
private:
  sf::Font font;
  sf::Text titleText;
  std::vector<sf::Text> menuItems;
  std::vector<std::wstring> menuLabels;
  int selectedIndex = 0;
  static constexpr size_t MENU_ITEMS_COUNT = 5;

  sf::Color backgroundColor = sf::Color(164, 144, 164);

  // Colors
  static constexpr uint8_t BACKGROUND_R = 99;
  static constexpr uint8_t BACKGROUND_G = 155;
  static constexpr uint8_t BACKGROUND_B = 255;

  sf::Color menuBackgroundColor = sf::Color(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B);

  static constexpr uint8_t TEXT_R = 251;  // 251, 242, 54
  static constexpr uint8_t TEXT_G = 242;  // 251, 242, 54
  static constexpr uint8_t TEXT_B = 54;   // 251, 242, 54

  sf::Color textColor = sf::Color(TEXT_R, TEXT_G, TEXT_B);

  static constexpr uint8_t BORDER_R = 95;
  static constexpr uint8_t BORDER_G = 205;
  static constexpr uint8_t BORDER_B = 228;

  sf::Color borderColor = sf::Color(BORDER_R, BORDER_G, BORDER_B);

  // Draw main menu rectangle
  sf::RectangleShape menuRect;
  float scaleRelativeFactor = 0.8f;

  sf::Vector2f menuRectSize = sf::Vector2f(800.0f * scaleRelativeFactor, 600.0f * scaleRelativeFactor);

  void drawMenuBackground(sf::RenderWindow& window, const sf::Text& text) const;

public:
  explicit MainMenu(sf::RenderWindow& win, Game& gameRef);

  void processEvents(const sf::Event& event) override;
  void update() override;
  void render() override;

private:
  void renderMenuRect();
  void renderTitle();
  void renderMenuItems();
};
