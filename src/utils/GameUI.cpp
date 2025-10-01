#include "GameUI.hpp"
#include "Digits.hpp"
#include "ResourceLoader.hpp"

namespace utils {

GameUI::GameUI() : scale(1.0f), color(sf::Color::White) {
  texture = ResourceLoader::getTexture(TextureType::GameUI);
  digits = Digits();
}

void GameUI::setScale(float value) const {
  scale = value;
}

void GameUI::setColor(const sf::Color& color) {
  this->color = color;
}

void GameUI::render(sf::RenderTarget& target, const sf::Vector2f& position, int level, int apples, int speed) const {
  renderLevel(target, position, level);
  renderApples(target, sf::Vector2f(position.x, position.y + getElementHeight() + 50), apples);
  renderSpeed(target, sf::Vector2f(position.x, position.y + getElementHeight() * 2 + 50), speed);
}

void GameUI::renderLevel(sf::RenderTarget& target, const sf::Vector2f& position, int level) const {
  sf::IntRect levelTextRect(sf::Vector2i(0, LEVEL_Y), sf::Vector2i(LEVEL_WIDTH, ELEMENT_HEIGHT));
  renderTextElement(target, position, levelTextRect, level, LEVEL_WIDTH);

  // Render the numeric value next to the text
  sf::Vector2f valuePosition(position.x + 148 * scale, position.y);
  digits.setScale(scale);
  digits.renderNumber(target, level, valuePosition);
}

void GameUI::renderApples(sf::RenderTarget& target, const sf::Vector2f& position, int apples) const {
  sf::IntRect applesTextRect(sf::Vector2i(0, APPLES_Y), sf::Vector2i(APPLES_WIDTH, ELEMENT_HEIGHT + 8));

  renderTextElement(target, position, applesTextRect, apples, APPLES_WIDTH);

  sf::Vector2f valuePosition(position.x + 148 * scale, position.y + 8 * scale);
  digits.setScale(scale);
  digits.renderNumber(target, apples, valuePosition);
}

void GameUI::renderSpeed(sf::RenderTarget& target, const sf::Vector2f& position, int speed) const {
  sf::IntRect speedTextRect(sf::Vector2i(0, SPEED_Y), sf::Vector2i(SPEED_WIDTH, ELEMENT_HEIGHT + 16));

  renderTextElement(target, position, speedTextRect, speed, SPEED_WIDTH);

  sf::Vector2f valuePosition(position.x + 148 * scale, position.y + 16 * scale);
  digits.setScale(scale);
  digits.renderNumber(target, speed, valuePosition);
}

float GameUI::getTotalHeight() const {
  return ELEMENT_HEIGHT * 3 * scale;  // 3 elements
}

float GameUI::getElementHeight() const {
  return ELEMENT_HEIGHT * scale;
}

float GameUI::getWidth() const {
  return SPEED_WIDTH * scale;  // Use the widest element
}

void GameUI::renderTextElement(sf::RenderTarget& target, const sf::Vector2f& position, const sf::IntRect& textRect,
                               int value, int textWidth) const {
  // Render the text label
  sf::Sprite textSprite(texture);
  textSprite.setTextureRect(textRect);
  textSprite.setPosition(position);
  textSprite.setScale(sf::Vector2f(scale, scale));
  textSprite.setColor(color);
  target.draw(textSprite);
}

}  // namespace utils
