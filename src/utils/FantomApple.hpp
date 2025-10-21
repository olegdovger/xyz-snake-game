#pragma once
#include "GameItem.hpp"

class FantomApple : public GameItem {
public:
  FantomApple(sf::Vector2i position, float lifetimeMultiplier = 1.0f);

  void render(sf::RenderWindow& window, const GameGrid& grid) const override;
  int getPoints() const override { return 0; }
  int getSpeedBonus() const override { return 2; }
  float getSpeedBonusDuration() const override { return 0.0f; }
  bool hasSpecialEffects() const override { return true; }
  void applySpecialEffects(class Snake& snake) const override;

  int getPointsPerCell() const { return 10; }

  float getInvincibilityDuration() const { return 10.0f; }

private:
  static constexpr int POINTS_PER_CELL = 10;
  static constexpr int SPEED_BONUS = 2;
  static constexpr float INVINCIBILITY_DURATION = 10.0f;
};
