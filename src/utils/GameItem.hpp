#pragma once
#include <SFML/Graphics.hpp>

class GameGrid;

class GameItem {
public:
  GameItem(sf::Vector2i position, float lifetime);

  virtual ~GameItem() = default;

  virtual bool update(float deltaTime);

  virtual void render(sf::RenderWindow& window, const GameGrid& grid) const = 0;

  unsigned char getAlpha() const;

  bool checkCollision(sf::Vector2i position) const;

  sf::Vector2i getPosition() const { return position; }

  bool isExpired() const { return expired; }

  virtual int getPoints() const = 0;

  virtual int getSpeedBonus() const = 0;

  virtual float getSpeedBonusDuration() const = 0;

  virtual bool hasSpecialEffects() const = 0;

  virtual void applySpecialEffects(class Snake& snake) const = 0;

protected:
  sf::Sprite sprite;
  sf::Vector2i position;
  float lifetime;
  float remainingTime;
  bool expired;
};
