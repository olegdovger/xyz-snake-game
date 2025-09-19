#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

namespace utils {
class EventLogger {
public:
  static void logEvent(const sf::Event& event);
  static void setDebugMode(const bool enabled);

private:
  static bool debugMode;
};
}  // namespace utils
