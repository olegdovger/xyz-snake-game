#pragma once
#include <SFML/Graphics.hpp>
#include "Digits.hpp"

class GameUI {
public:
  GameUI();

  void setScale(float scale) const;

  void setColor(const sf::Color& color);

  void render(sf::RenderTarget& target, const sf::Vector2f& position) const;

  void renderScore(sf::RenderTarget& target, const sf::Vector2f& position) const;
  void renderApples(sf::RenderTarget& target, const sf::Vector2f& position) const;
  void renderSpeed(sf::RenderTarget& target, const sf::Vector2f& position) const;

  float getTotalHeight() const;

  float getElementHeight() const;

  float getWidth() const;

  void setScore(int score);
  void setApples(int apples);
  void setSpeed(int speed);

  int getScore() const;
  int getApples() const;
  int getSpeed() const;

private:
  sf::Texture texture;
  mutable Digits digits;
  mutable float scale;
  sf::Color color;

  long long score;
  int apples;
  int speed;

  static constexpr int ELEMENT_HEIGHT = 36;
  static constexpr int SCORE_Y = 0;
  static constexpr int APPLES_Y = 36;
  static constexpr int SPEED_Y = 72;

  static constexpr int SCORE_WIDTH = 68;
  static constexpr int APPLES_WIDTH = 108;
  static constexpr int SPEED_WIDTH = 132;

  void renderTextElement(sf::RenderTarget& target, const sf::Vector2f& position, const sf::IntRect& textRect) const;
};
