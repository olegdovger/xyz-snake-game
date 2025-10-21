#pragma once
#include <SFML/Graphics.hpp>

namespace shape {

float getScale(const sf::Vector2f objectSize, const sf::Vector2u windowSize, float scaleFactor = 0.8f);

sf::Vector2f getPosition(const sf::Vector2f objectSize, const sf::Vector2u windowSize, float scale = 1.0f);

}  // namespace shape
