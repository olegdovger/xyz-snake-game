#include "DebugUI.hpp"
#include <SFML/Graphics.hpp>
#include "ResourceLoader.hpp"

std::string DebugUI::debugLines;
bool DebugUI::isInitialized = false;

void DebugUI::initialize(sf::RenderWindow& window) {
  if (isInitialized)
    return;

  isInitialized = true;
}

void DebugUI::addDebugText(const std::string& text) {
  debugLines = text;
}

void DebugUI::render(sf::RenderWindow& window) {
  if (!isInitialized || debugLines.empty())
    return;

  const sf::Font& font = ResourceLoader::getFont(FontType::DebugFont);

  const float fontSize = 32.0f;

  sf::Text text(font);
  text.setString(debugLines);
  text.setCharacterSize(static_cast<unsigned int>(fontSize));
  text.setFillColor(sf::Color::Black);
  text.setOutlineColor(sf::Color::Transparent);
  text.setOutlineThickness(1.0f);
  text.setPosition(sf::Vector2f(10.0f, 10.0f));

  window.draw(text);
}

void DebugUI::clear() {
  debugLines.clear();
}
