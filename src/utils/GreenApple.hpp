#pragma once
#include "GameItem.hpp"

/**
 * @brief Green apple implementation
 * 
 * Gives +5 speed bonus for 5 seconds and 10 points.
 */
class GreenApple : public GameItem {
public:
  /**
     * @brief Constructor
     * @param position Grid position
     */
  GreenApple(sf::Vector2i position);

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
