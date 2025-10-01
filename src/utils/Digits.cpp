#include "Digits.hpp"
#include <iomanip>
#include <sstream>
#include "ResourceLoader.hpp"

namespace utils {

Digits::Digits() : scale(1.0f), color(sf::Color::White) {
  texture = ResourceLoader::getTexture(TextureType::Digits);
}

void Digits::setScale(float scale) const {
  this->scale = scale;
}

void Digits::setColor(const sf::Color& color) {
  this->color = color;
}

void Digits::renderDigit(sf::RenderTarget& target, int digit, const sf::Vector2f& position) const {
  if (digit < 0 || digit > 9) {
    return;
  }

  sf::Sprite sprite(texture);
  sprite.setTextureRect(getDigitRect(digit));
  sprite.setPosition(position);
  sprite.setScale(sf::Vector2f(scale, scale));
  sprite.setColor(color);

  target.draw(sprite);
}

void Digits::renderNumber(sf::RenderTarget& target, int number, const sf::Vector2f& position, int maxDigits) const {
  std::stringstream ss;
  if (maxDigits > 0) {
    ss << std::setfill('0') << std::setw(maxDigits) << number;
  } else {
    ss << number;
  }

  renderDigitString(target, ss.str(), position);
}

void Digits::renderDigitString(sf::RenderTarget& target, const std::string& digitString,
                               const sf::Vector2f& position) const {
  float currentX = position.x;
  const float digitSpacing = 4.0f * scale;  // 2 pixel spacing between digits

  for (char c : digitString) {
    if (c >= '0' && c <= '9') {
      int digit = c - '0';
      renderDigit(target, digit, sf::Vector2f(currentX, position.y));
      currentX += getDigitWidth() + digitSpacing;
    }
  }
}

float Digits::getDigitWidth() const {
  return DIGIT_WIDTH * scale;
}

float Digits::getDigitHeight() const {
  return DIGIT_HEIGHT * scale;
}

float Digits::getNumberWidth(int digitCount) const {
  if (digitCount <= 0)
    return 0.0f;
  const float digitSpacing = 2.0f * scale;
  return digitCount * getDigitWidth() + (digitCount - 1) * digitSpacing;
}

sf::IntRect Digits::getDigitRect(int digit) const {
  // Map digits to sprite positions: 0->9, 1->0, 2->1, ..., 9->8
  int spriteIndex = (digit == 0) ? 9 : digit - 1;
  int x = spriteIndex * TOTAL_DIGIT_WIDTH;
  return sf::IntRect(sf::Vector2i(x, 0), sf::Vector2i(DIGIT_WIDTH, DIGIT_HEIGHT));
}

}  // namespace utils
