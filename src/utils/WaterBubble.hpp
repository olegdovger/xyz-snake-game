#pragma once
#include "GameItem.hpp"

class WaterBubble : public GameItem {
public:
  WaterBubble(sf::Vector2i position, float lifetimeMultiplier = 1.0f);

  void render(sf::RenderWindow& window, const GameGrid& grid) const override;
  int getPoints() const override { return 100; }
  int getSpeedBonus() const override { return 0; }
  float getSpeedBonusDuration() const override { return 0.0f; }
  bool hasSpecialEffects() const override { return true; }
  void applySpecialEffects(class Snake& snake) const override;

private:
  static constexpr int POINTS = 100;
  static constexpr float EFFECT_DURATION = 5.0f;
  static constexpr float SPEED_MULTIPLIER = 0.5f;
};
