#include "GameUI.hpp"
#include "Digits.hpp"
#include "ResourceLoader.hpp"

GameUI::GameUI() : scale(1.0f), color(sf::Color::White), score(0), apples(0), speed(0) {
  texture = ResourceLoader::getTexture(TextureType::GameUI);
  digits = Digits();
}

void GameUI::setScale(float value) const {
  scale = value;
}

void GameUI::setColor(const sf::Color& color) {
  this->color = color;
}

void GameUI::render(sf::RenderTarget& target, const sf::Vector2f& position) const {
  renderScore(target, position);
  renderApples(target, sf::Vector2f(position.x, position.y + getElementHeight()));
  renderSpeed(target, sf::Vector2f(position.x, position.y + getElementHeight() * 2));
}

void GameUI::renderScore(sf::RenderTarget& target, const sf::Vector2f& position) const {
  sf::IntRect scoreTextRect(sf::Vector2i(0, SCORE_Y), sf::Vector2i(SCORE_WIDTH, ELEMENT_HEIGHT));

  renderTextElement(target, position, scoreTextRect);

  sf::Vector2f valuePosition(position.x + 148 * scale, position.y);
  digits.setScale(scale);
  digits.renderNumber(target, score, valuePosition);
}

void GameUI::renderApples(sf::RenderTarget& target, const sf::Vector2f& position) const {
  sf::IntRect applesTextRect(sf::Vector2i(0, APPLES_Y), sf::Vector2i(APPLES_WIDTH, ELEMENT_HEIGHT + 8));

  renderTextElement(target, position, applesTextRect);

  sf::Vector2f valuePosition(position.x + 148 * scale, position.y + 8 * scale);
  digits.setScale(scale);
  digits.renderNumber(target, apples, valuePosition);
}

void GameUI::renderSpeed(sf::RenderTarget& target, const sf::Vector2f& position) const {
  sf::IntRect speedTextRect(sf::Vector2i(0, SPEED_Y), sf::Vector2i(SPEED_WIDTH, ELEMENT_HEIGHT + 16));

  renderTextElement(target, position, speedTextRect);

  sf::Vector2f valuePosition(position.x + 148 * scale, position.y + 16 * scale);
  digits.setScale(scale);
  digits.renderNumber(target, speed, valuePosition);
}

float GameUI::getTotalHeight() const {
  return ELEMENT_HEIGHT * 4 * scale;
}

float GameUI::getElementHeight() const {
  return ELEMENT_HEIGHT * scale;
}

float GameUI::getWidth() const {
  return SPEED_WIDTH * scale;
}

void GameUI::setApples(int apples) {
  this->apples = apples;
}

void GameUI::setSpeed(int speed) {
  this->speed = speed;
}

void GameUI::setScore(int score) {
  this->score = score;
}

int GameUI::getApples() const {
  return apples;
}

int GameUI::getSpeed() const {
  return speed;
}

int GameUI::getScore() const {
  return score;
}

void GameUI::renderTextElement(sf::RenderTarget& target, const sf::Vector2f& position,
                               const sf::IntRect& textRect) const {
  sf::Sprite textSprite(texture);
  textSprite.setTextureRect(textRect);
  textSprite.setPosition(position);
  textSprite.setScale(sf::Vector2f(scale, scale));
  textSprite.setColor(color);
  target.draw(textSprite);
}
