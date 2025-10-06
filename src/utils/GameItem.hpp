#pragma once
#include <SFML/Graphics.hpp>

class GameGrid;

/**
 * @brief Base class for all game item types
 * 
 * Provides common functionality for all game items including position,
 * lifetime management, and rendering.
 */
class GameItem {
public:
  /**
     * @brief Constructor
     * @param position Grid position of the game item
     * @param lifetime Time in seconds before item expires
     */
  GameItem(sf::Vector2i position, float lifetime);

  /**
     * @brief Virtual destructor
     */
  virtual ~GameItem() = default;

  /**
     * @brief Update game item state
     * @param deltaTime Time elapsed since last update
     * @return true if item is still active, false if expired
     */
  virtual bool update(float deltaTime);

  /**
     * @brief Render the game item
     * @param window Render window
     * @param grid Game grid for position conversion
     */
  virtual void render(sf::RenderWindow& window, const GameGrid& grid) const = 0;

  /**
     * @brief Get alpha value based on remaining lifetime
     * @return Alpha value (0-255)
     */
  unsigned char getAlpha() const;

  /**
     * @brief Check if game item collides with given position
     * @param position Position to check
     * @return true if collision detected
     */
  bool checkCollision(sf::Vector2i position) const;

  /**
     * @brief Get game item position
     * @return Grid position
     */
  sf::Vector2i getPosition() const { return position; }

  /**
     * @brief Check if game item is expired
     * @return true if expired
     */
  bool isExpired() const { return expired; }

  /**
     * @brief Get points awarded for eating this item
     * @return Point value
     */
  virtual int getPoints() const = 0;

  /**
     * @brief Get speed bonus from this item
     * @return Speed bonus (0 = no change)
     */
  virtual int getSpeedBonus() const = 0;

  /**
     * @brief Get speed bonus duration
     * @return Duration in seconds (0 = permanent)
     */
  virtual float getSpeedBonusDuration() const = 0;

  /**
     * @brief Check if item has special effects
     * @return true if has special effects
     */
  virtual bool hasSpecialEffects() const = 0;

  /**
     * @brief Apply special effects to snake
     * @param snake Reference to snake
     */
  virtual void applySpecialEffects(class Snake& snake) const = 0;

protected:
  sf::Sprite sprite;
  sf::Vector2i position;
  float lifetime;
  float remainingTime;
  bool expired;
};
