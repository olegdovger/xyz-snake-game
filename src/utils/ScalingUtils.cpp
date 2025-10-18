#include "ScalingUtils.hpp"

namespace shape {

float getScale(const sf::Vector2f objectSize, const sf::Vector2u windowSize, float scaleFactor) {
  const float scaleX = (windowSize.x * scaleFactor) / objectSize.x;
  const float scaleY = (windowSize.y * scaleFactor) / objectSize.y;
  return std::min(scaleX, scaleY);
}

sf::Vector2f getPosition(const sf::Vector2f objectSize, const sf::Vector2u windowSize, float scale) {
  const float scaledWidth = objectSize.x * scale;
  const float scaledHeight = objectSize.y * scale;

  const float positionX = (windowSize.x - scaledWidth) / 2.0f;
  const float positionY = (windowSize.y - scaledHeight) / 2.0f;

  return sf::Vector2f(positionX, positionY);
}

}  // namespace shape
