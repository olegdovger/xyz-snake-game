#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <string>
#include <vector>
#include "../Game.hpp"
#include "../Screen.hpp"
#include "../utils/MenuSoundManager.hpp"

class MainMenu : public Screen {
private:
  sf::RectangleShape screenRect;
  sf::Font font;
  sf::Text titleText;
  std::vector<sf::Text> menuItems;
  std::vector<std::wstring> menuLabels;
  int selectedIndex = 0;
  sf::Vector2f originSize = sf::Vector2f(400.0f, 400.0f);

  static constexpr size_t MENU_ITEMS_COUNT = 5;

  sf::Color backgroundColor = sf::Color(164, 144, 164);

  sf::Color menuBackgroundColor = MenuColors::BACKGROUND_COLOR;
  sf::Color textColor = MenuColors::TEXT_COLOR;
  sf::Color borderColor = MenuColors::BORDER_COLOR;

  float scaleRelativeFactor = 0.8f;

  sf::Vector2f menuRectSize = originSize * scaleRelativeFactor;

  MenuSoundManager soundManager;

  void drawMenuBackground(sf::RenderWindow& window, const sf::Text& text) const;

  void initializeMenuItems();
  void renderMenuRect();
  void renderTitle();
  void renderMenuItems();

public:
  explicit MainMenu(sf::RenderWindow& win, Game& gameRef);

  void processEvents(const sf::Event& event) override;
  void update() override;
  void render() override;
};
