#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class CountdownTimer {
private:
  sf::Clock clock;
  sf::Text countdownText;
  sf::Sound countdownSound;
  int totalSeconds;
  int currentSeconds;
  bool isActive;
  bool isFinished;
  bool soundEnabled;

  void updateText();

public:
  explicit CountdownTimer(int totalSeconds = 3, bool soundEnabled = false);

  ~CountdownTimer() = default;

  void start();

  void update();

  void render(sf::RenderWindow& window);

  [[nodiscard]] bool getIsActive() const { return isActive; }

  [[nodiscard]] bool getIsFinished() const { return isFinished; }

  [[nodiscard]] int getRemainingSeconds() const { return currentSeconds; }

  void reset();
  void setDuration(int seconds);
  void setPosition(const sf::Vector2f& position);
  void setSoundEnabled(bool enabled);
};
