#pragma once
#include "GameItem.hpp"

class GreenApple : public GameItem {
public:
  GreenApple(sf::Vector2i position, float lifetimeMultiplier = 1.0f);

  void render(sf::RenderWindow& window, const GameGrid& grid) const override;
  int getPoints() const override { return 10; }
  int getSpeedBonus() const override { return 5; }
  float getSpeedBonusDuration() const override { return 5.0f; }
  bool hasSpecialEffects() const override { return true; }
  void applySpecialEffects(class Snake& snake) const override;

private:
  static constexpr int POINTS = 10;
  static constexpr int SPEED_BONUS = 5;
  static constexpr float SPEED_DURATION = 5.0f;
};
