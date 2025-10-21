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
#include "../utils/MenuSoundManager.hpp"

class PauseScreen final : public Screen {
private:
  sf::RectangleShape screenRect;
  sf::Font font;
  sf::Text titleText;
  sf::Text backText;
  std::vector<sf::Text> menuItems;
  int selectedIndex = 0;
  sf::Vector2f originSize = sf::Vector2f(400.0f, 400.0f);

  static constexpr size_t MENU_ITEMS_COUNT = 3;

  sf::Color backgroundColor = sf::Color(164, 144, 164);

  sf::Color menuBackgroundColor = MenuColors::BACKGROUND_COLOR;
  sf::Color textColor = MenuColors::TEXT_COLOR;
  sf::Color borderColor = MenuColors::BORDER_COLOR;

  float scaleRelativeFactor = 0.6f;

  sf::Vector2f menuRectSize = originSize * scaleRelativeFactor;

  // Menu sound manager
  MenuSoundManager soundManager;

  // Sound settings
  bool soundEnabled = true;

  void initializeMenuItems();
  void renderMenuRect();
  void renderTitle();
  void renderMenuItems();
  void renderBackButton();

public:
  explicit PauseScreen(sf::RenderWindow& win, Game& gameRef);

  void processEvents(const sf::Event& event) override;
  void update() override;
  void render() override;
};
