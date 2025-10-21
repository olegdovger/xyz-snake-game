#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Digits {
public:
  explicit Digits();

  void setScale(float scale) const;

  void setColor(const sf::Color& color);

  void renderDigit(sf::RenderTarget& target, int digit, const sf::Vector2f& position) const;

  void renderNumber(sf::RenderTarget& target, int number, const sf::Vector2f& position, int maxDigits = 0) const;

  void renderDigitString(sf::RenderTarget& target, const std::string& digitString, const sf::Vector2f& position) const;

  float getDigitWidth() const;

  float getDigitHeight() const;

  float getNumberWidth(int digitCount) const;

private:
  sf::Texture texture;
  mutable float scale;
  sf::Color color;

  static constexpr int DIGIT_WIDTH = 12;
  static constexpr int DIGIT_HEIGHT = 36;
  static constexpr int DIGIT_SPACING = 4;
  static constexpr int TOTAL_DIGIT_WIDTH = DIGIT_WIDTH + DIGIT_SPACING;

  sf::IntRect getDigitRect(int digit) const;
};
