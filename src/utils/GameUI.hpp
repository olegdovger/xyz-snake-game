#pragma once
#include <SFML/Graphics.hpp>
#include "Digits.hpp"

namespace utils {

/**
 * @brief Class for rendering game UI elements
 * 
 * The GameUI class provides functionality to render game information
 * including level, apples count, and speed using text sprites and digits.
 * All text elements are arranged vertically with 36 pixel height each.
 */
class GameUI {
public:
  /**
     * @brief Constructor
     */
  GameUI();

  /**
   * @brief Set the scale factor for rendering
   * @param scale Scale factor (1.0 = original size)
   */
  void setScale(float scale) const;

  /**
     * @brief Set the color tint for the UI elements
     * @param color Color to apply to the UI elements
     */
  void setColor(const sf::Color& color);

  /**
     * @brief Render the complete UI
     * @param target Render target
     * @param position Position to render at
     * @param level Current level
     * @param apples Current apple count
     * @param speed Current speed
     */
  void render(sf::RenderTarget& target, const sf::Vector2f& position, int level, int apples, int speed) const;

  /**
     * @brief Render level information
     * @param target Render target
     * @param position Position to render at
     * @param level Current level
     */
  void renderLevel(sf::RenderTarget& target, const sf::Vector2f& position, int level) const;

  /**
     * @brief Render apples count
     * @param target Render target
     * @param position Position to render at
     * @param apples Current apple count
     */
  void renderApples(sf::RenderTarget& target, const sf::Vector2f& position, int apples) const;

  /**
     * @brief Render speed information
     * @param target Render target
     * @param position Position to render at
     * @param speed Current speed
     */
  void renderSpeed(sf::RenderTarget& target, const sf::Vector2f& position, int speed) const;

  /**
     * @brief Get the total height of all UI elements
     * @return Total height in pixels (scaled)
     */
  float getTotalHeight() const;

  /**
     * @brief Get the height of a single UI element
     * @return Height in pixels (scaled)
     */
  float getElementHeight() const;

  /**
     * @brief Get the width of the UI elements
     * @return Width in pixels (scaled)
     */
  float getWidth() const;

private:
  sf::Texture texture;
  mutable Digits digits;
  mutable float scale;
  sf::Color color;

  static constexpr int ELEMENT_HEIGHT = 36;
  static constexpr int LEVEL_Y = 0;
  static constexpr int APPLES_Y = 36;
  static constexpr int SPEED_Y = 72;
  static constexpr int LEVEL_WIDTH = 116;
  static constexpr int APPLES_WIDTH = 108;
  static constexpr int SPEED_WIDTH = 132;

  /**
     * @brief Render a text element
     * @param target Render target
     * @param position Position to render at
     * @param textRect Texture rectangle for the text sprite
     * @param value Numeric value to display
     * @param textWidth Width of the text sprite
     */
  void renderTextElement(sf::RenderTarget& target, const sf::Vector2f& position, const sf::IntRect& textRect, int value,
                         int textWidth) const;
};

}  // namespace utils
