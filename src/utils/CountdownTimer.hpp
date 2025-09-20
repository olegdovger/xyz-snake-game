#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace utils {

/**
 * @brief Class for handling game countdown timer
 */
class CountdownTimer {
private:
  sf::Clock clock;
  sf::Text countdownText;
  int totalSeconds;
  int currentSeconds;
  bool isActive;
  bool isFinished;
  bool soundEnabled;

  void updateText();

public:
  /**
   * @brief Constructor
   * @param totalSeconds Total countdown duration in seconds
   * @param soundEnabled Whether to play countdown sound
   */
  explicit CountdownTimer(int totalSeconds = 3, bool soundEnabled = false);

  /**
   * @brief Destructor
   */
  ~CountdownTimer() = default;

  /**
   * @brief Start the countdown timer
   */
  void start();

  /**
   * @brief Update the countdown timer (call each frame)
   */
  void update();

  /**
   * @brief Render the countdown text
   * @param window Render window to draw to
   */
  void render(sf::RenderWindow& window);

  /**
   * @brief Check if countdown is active
   * @return true if countdown is running, false otherwise
   */
  [[nodiscard]] bool getIsActive() const { return isActive; }

  /**
   * @brief Check if countdown has finished
   * @return true if countdown is finished, false otherwise
   */
  [[nodiscard]] bool getIsFinished() const { return isFinished; }

  /**
   * @brief Get remaining seconds
   * @return remaining seconds
   */
  [[nodiscard]] int getRemainingSeconds() const { return currentSeconds; }

  /**
   * @brief Reset the countdown timer
   */
  void reset();

  /**
   * @brief Set the countdown duration
   * @param seconds New duration in seconds
   */
  void setDuration(int seconds);

  /**
   * @brief Set the position of the countdown text
   * @param position Position to center the text at
   */
  void setPosition(const sf::Vector2f& position);
};

}  // namespace utils
