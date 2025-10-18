#pragma once
#include <SFML/Graphics.hpp>

namespace shape {

// Get scale factor for object to fit in window
float getScale(const sf::Vector2f objectSize, const sf::Vector2u windowSize, float scaleFactor = 0.8f);

// Get centered position for object
sf::Vector2f getPosition(const sf::Vector2f objectSize, const sf::Vector2u windowSize, float scale = 1.0f);

}  // namespace shape
