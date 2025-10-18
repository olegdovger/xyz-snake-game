#pragma once
#include "GameItem.hpp"

/**
 * @brief Fantom apple implementation
 * 
 * Gives invincibility for 10 seconds (no wall collisions inside board),
 * +10 points for each cell passed, can eat all apples ignoring speed reduction,
 * and +2 permanent speed bonus.
 */
class FantomApple : public GameItem {
public:
  /**
     * @brief Constructor
     * @param position Grid position
     * @param lifetimeMultiplier Multiplier for apple lifetime
     */
  FantomApple(sf::Vector2i position, float lifetimeMultiplier = 1.0f);

  void render(sf::RenderWindow& window, const GameGrid& grid) const override;
  int getPoints() const override { return 0; }  // Points are per-cell, not per apple
  int getSpeedBonus() const override { return 2; }
  float getSpeedBonusDuration() const override { return 0.0f; }  // Permanent
  bool hasSpecialEffects() const override { return true; }
  void applySpecialEffects(class Snake& snake) const override;

  /**
     * @brief Get points per cell passed during invincibility
     * @return Points per cell
     */
  int getPointsPerCell() const { return 10; }

  /**
     * @brief Get invincibility duration
     * @return Duration in seconds
     */
  float getInvincibilityDuration() const { return 10.0f; }

private:
  static constexpr int POINTS_PER_CELL = 10;
  static constexpr int SPEED_BONUS = 2;
  static constexpr float INVINCIBILITY_DURATION = 10.0f;
};
