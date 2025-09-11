#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "ResourceManager.hpp"

namespace utils {
class DebugUI {
public:
  static void initialize(sf::RenderWindow& window);
  static void addDebugText(const std::string& text);
  static void render(sf::RenderWindow& window);
  static void clear();

private:
  static std::string debugLines;
  static bool isInitialized;
};
}  // namespace utils
