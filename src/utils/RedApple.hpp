#pragma once
#include "GameItem.hpp"

/**
 * @brief Red apple implementation
 * 
 * Gives +1 speed bonus permanently and 50 points.
 * Has limited lifetime based on board traversal time.
 */
class RedApple : public GameItem {
public:
  /**
     * @brief Constructor
     * @param position Grid position
     * @param boardWidth Board width in cells
     * @param boardHeight Board height in cells
     * @param snakeSpeed Current snake speed
     */
  RedApple(sf::Vector2i position, int boardWidth, int boardHeight, float snakeSpeed);

  void render(sf::RenderWindow& window, const GameGrid& grid) const override;
  int getPoints() const override { return 50; }
  int getSpeedBonus() const override { return 1; }
  float getSpeedBonusDuration() const override { return 0.0f; }  // Permanent
  bool hasSpecialEffects() const override { return true; }
  void applySpecialEffects(class Snake& snake) const override;

private:
  static float calculateLifetime(int boardWidth, int boardHeight, float snakeSpeed);
  static constexpr int POINTS = 50;
  static constexpr int SPEED_BONUS = 1;
};
