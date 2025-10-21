#pragma once
#include "GameItem.hpp"

class RedApple : public GameItem {
public:
  RedApple(sf::Vector2i position, int boardWidth, int boardHeight, float snakeSpeed, float lifetimeMultiplier = 1.0f);

  void render(sf::RenderWindow& window, const GameGrid& grid) const override;
  int getPoints() const override { return 50; }
  int getSpeedBonus() const override { return 1; }
  float getSpeedBonusDuration() const override { return 0.0f; }
  bool hasSpecialEffects() const override { return true; }
  void applySpecialEffects(class Snake& snake) const override;

private:
  static float calculateLifetime(int boardWidth, int boardHeight, float snakeSpeed);
  static constexpr int POINTS = 50;
  static constexpr int SPEED_BONUS = 1;
};
