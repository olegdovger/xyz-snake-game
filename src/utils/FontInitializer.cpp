#include "FontInitializer.hpp"
#include "ResourceLoader.hpp"

void FontInitializer::initializeTitleText(sf::Text& text, const sf::Font& font, const std::wstring& title) {
  text.setString(title);
  text.setFont(font);
  text.setCharacterSize(40);
  text.setFillColor(sf::Color::White);
  text.setStyle(sf::Text::Bold);
}

void FontInitializer::initializeBackText(sf::Text& text, const sf::Font& font, unsigned int characterSize) {
  text.setString(L"Назад (Escape)");
  text.setFont(font);
  text.setCharacterSize(characterSize);
  text.setFillColor(sf::Color::White);
  text.setStyle(sf::Text::Bold);
}

void FontInitializer::initializeMenuItemText(sf::Text& text, const sf::Font& font, const std::wstring& itemText,
                                             unsigned int characterSize) {
  text.setString(itemText);
  text.setFont(font);
  text.setCharacterSize(characterSize);
  text.setFillColor(sf::Color::White);
}

const sf::Font& FontInitializer::getDefaultFont() {
  return ResourceLoader::getFont(FontType::UIFont);
}

const sf::Font& FontInitializer::getDebugFont() {
  return ResourceLoader::getFont(FontType::DebugFont);
}
