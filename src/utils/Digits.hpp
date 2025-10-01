#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace utils {

/**
 * @brief Class for rendering digits from a sprite sheet
 * 
 * The Digits class provides functionality to render individual digits or
 * sequences of numbers using a sprite sheet containing digits 0-9.
 * Each digit is 12x36 pixels with 4-pixel spacing between digits.
 */
class Digits {
public:
  /**
     * @brief Constructor
     */
  explicit Digits();

  /**
     * @brief Set the scale factor for rendering
     * @param scale Scale factor (1.0 = original size)
     */
  void setScale(float scale) const;

  /**
     * @brief Set the color tint for the digits
     * @param color Color to apply to the digits
     */
  void setColor(const sf::Color& color);

  /**
     * @brief Render a single digit
     * @param target Render target
     * @param digit Single digit (0-9)
     * @param position Position to render at
     */
  void renderDigit(sf::RenderTarget& target, int digit, const sf::Vector2f& position) const;

  /**
     * @brief Render a number as a sequence of digits
     * @param target Render target
     * @param number Number to render
     * @param position Position to render at
     * @param maxDigits Maximum number of digits to display (for padding with zeros)
     */
  void renderNumber(sf::RenderTarget& target, int number, const sf::Vector2f& position, int maxDigits = 0) const;

  /**
     * @brief Render a string of digits
     * @param target Render target
     * @param digitString String containing only digits
     * @param position Position to render at
     */
  void renderDigitString(sf::RenderTarget& target, const std::string& digitString, const sf::Vector2f& position) const;

  /**
     * @brief Get the width of a single digit
     * @return Width in pixels (scaled)
     */
  float getDigitWidth() const;

  /**
     * @brief Get the height of a single digit
     * @return Height in pixels (scaled)
     */
  float getDigitHeight() const;

  /**
     * @brief Get the width of a number with given digit count
     * @param digitCount Number of digits
     * @return Total width in pixels (scaled)
     */
  float getNumberWidth(int digitCount) const;

private:
  sf::Texture texture;
  mutable float scale;
  sf::Color color;

  static constexpr int DIGIT_WIDTH = 12;
  static constexpr int DIGIT_HEIGHT = 36;
  static constexpr int DIGIT_SPACING = 4;
  static constexpr int TOTAL_DIGIT_WIDTH = DIGIT_WIDTH + DIGIT_SPACING;

  /**
     * @brief Get the texture rectangle for a specific digit
     * @param digit Digit (0-9)
     * @return Texture rectangle for the digit
     */
  sf::IntRect getDigitRect(int digit) const;
};

}  // namespace utils
