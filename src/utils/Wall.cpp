#include "Wall.hpp"
#include <random>
#include "GameGrid.hpp"
#include "ResourceLoader.hpp"

Wall::Wall(const std::vector<sf::Vector2i>& positions, WallType type)
    : positions(positions),
      type(type),
      expired(false),
      currentPhase(WallPhase::Appearing),
      blinking(false),
      blinkCount(0) {

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

  // Update wall phase
  if (currentPhase == WallPhase::Appearing && elapsedTime >= APPEARANCE_DURATION) {
    currentPhase = WallPhase::Active;
  } else if (currentPhase == WallPhase::Active && elapsedTime >= lifetime - DISAPPEARANCE_DURATION) {
    currentPhase = WallPhase::Disappearing;
    startBlinking();
  }

  // Handle blinking during disappearance phase
  if (blinking && currentPhase == WallPhase::Disappearing) {
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

    // Position the sprite (position is row=y, col=x to match snake coordinate system)
    sf::Vector2f cellPosition = grid.getCellPosition(position.y, position.x);
    sf::Vector2f centerOffset(grid.getScaledCellSize() / 2.0f, grid.getScaledCellSize() / 2.0f);
    wallSprite.setPosition(cellPosition);

    // Scale the sprite to fit the cell
    sf::Vector2u textureSize = texture.getSize();
    float scale = grid.getScaledCellSize() / (float)textureSize.x;

    wallSprite.setScale(sf::Vector2f(scale, scale));

    // Apply phase-based effects
    if (currentPhase == WallPhase::Appearing) {
      // Fade in during appearance phase
      float elapsedTime = lifetimeClock.getElapsedTime().asSeconds();
      float alpha = (elapsedTime / APPEARANCE_DURATION) * 255.0f;
      wallSprite.setColor(sf::Color(255, 255, 255, static_cast<unsigned char>(alpha)));
    } else if (blinking && currentPhase == WallPhase::Disappearing) {
      // Blinking effect during disappearance
      wallSprite.setColor(getBlinkColor());
    } else {
      // Normal appearance during active phase
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

bool Wall::canCollide() const {
  // Wall can only collide during active phase
  return currentPhase == WallPhase::Active;
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
