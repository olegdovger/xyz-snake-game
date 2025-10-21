#pragma once
#include <SFML/Graphics.hpp>

class FontInitializer {
public:
  static void initializeTitleText(sf::Text& text, const sf::Font& font, const std::wstring& title);
  static void initializeBackText(sf::Text& text, const sf::Font& font, unsigned int characterSize = 14);
  static void initializeMenuItemText(sf::Text& text, const sf::Font& font, const std::wstring& itemText,
                                     unsigned int characterSize = 24);
  static const sf::Font& getDefaultFont();
  static const sf::Font& getDebugFont();
};
