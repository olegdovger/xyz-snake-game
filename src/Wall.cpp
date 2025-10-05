#include "Wall.hpp"
#include <random>
#include "utils/GameGrid.hpp"
#include "utils/ResourceLoader.hpp"

Wall::Wall(const std::vector<sf::Vector2i>& positions, WallType type)
    : positions(positions), type(type), expired(false), blinking(false), blinkCount(0) {

  // Generate random lifetime between 10-20 seconds
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<float> dis(10.0f, 20.0f);
  lifetime = dis(gen);

  loadTexture();
  lifetimeClock.restart();
}

void Wall::update(float deltaTime) {
  if (expired)
    return;

  float elapsedTime = lifetimeClock.getElapsedTime().asSeconds();

  // Check if wall should start blinking (3 seconds before expiration)
  if (elapsedTime >= lifetime - 3.0f && !blinking) {
    startBlinking();
  }

  // Handle blinking
  if (blinking) {
    if (blinkTimer.getElapsedTime().asSeconds() >= BLINK_DURATION) {
      blinkCount++;
      blinkTimer.restart();

      if (blinkCount >= MAX_BLINKS) {
        expired = true;
      }
    }
  }

  // Check if wall has expired
  if (elapsedTime >= lifetime) {
    expired = true;
  }
}

void Wall::render(sf::RenderWindow& window, const GameGrid& grid) const {
  if (expired)
    return;

  for (const auto& position : positions) {
    sf::Sprite wallSprite(texture);

    // Position the sprite
    sf::Vector2f cellPosition = grid.getCellPosition(position.y, position.x);
    sf::Vector2f centerOffset(grid.getScaledCellSize() / 2.0f, grid.getScaledCellSize() / 2.0f);
    wallSprite.setPosition(cellPosition + centerOffset);

    // Scale the sprite to fit the cell
    float scale = grid.getScaledCellSize() / 28.0f;  // Assuming 28px texture size
    wallSprite.setScale(sf::Vector2f(scale, scale));

    // Apply blinking effect
    if (blinking) {
      wallSprite.setColor(getBlinkColor());
    } else {
      wallSprite.setColor(sf::Color::White);
    }

    window.draw(wallSprite);
  }
}

float Wall::getRemainingTime() const {
  if (expired)
    return 0.0f;
  return std::max(0.0f, lifetime - lifetimeClock.getElapsedTime().asSeconds());
}

bool Wall::checkCollisionWithPosition(sf::Vector2i position) const {
  return std::find(positions.begin(), positions.end(), position) != positions.end();
}

void Wall::loadTexture() {
  TextureType textureType;
  switch (type) {
    case WallType::Wall_1:
      textureType = TextureType::Wall_1;
      break;
    case WallType::Wall_2:
      textureType = TextureType::Wall_2;
      break;
    case WallType::Wall_3:
      textureType = TextureType::Wall_3;
      break;
    case WallType::Wall_4:
      textureType = TextureType::Wall_4;
      break;
  }

  const auto& textureRef = ResourceLoader::getTexture(textureType);
  texture = textureRef;
}

void Wall::startBlinking() {
  blinking = true;
  blinkCount = 0;
  blinkTimer.restart();
}

sf::Color Wall::getBlinkColor() const {
  // Calculate animated alpha based on time
  float time = blinkTimer.getElapsedTime().asSeconds();
  float alpha = 128 + 127 * std::sin(time * 3.14159f * 4.0f);  // Oscillates between 128 and 255
  return sf::Color(255, 255, 255, static_cast<unsigned char>(alpha));
}
