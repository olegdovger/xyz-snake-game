#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include "PausableClock.hpp"
#include "difficulty/DifficultySettings.hpp"

class GameGrid;

enum class WallPhase { Appearing, Active, Disappearing };

class Wall {
public:
  enum class WallType { Wall_1, Wall_2, Wall_3, Wall_4 };

  explicit Wall(const std::vector<sf::Vector2i>& positions, WallType type = WallType::Wall_1,
                const DifficultySettings* difficulty = nullptr);

  // Lifecycle
  void update(float deltaTime);
  bool isExpired() const { return expired; }
  bool isBlinking() const { return blinking; }

  // Rendering
  void render(sf::RenderWindow& window, const GameGrid& grid) const;

  // Properties
  const std::vector<sf::Vector2i>& getPositions() const { return positions; }
  WallType getType() const { return type; }

  // Collision detection
  bool checkCollisionWithPosition(sf::Vector2i position) const;
  bool canCollide() const;  // Returns true only during active phase
  WallPhase getCurrentPhase() const { return currentPhase; }

private:
  std::vector<sf::Vector2i> positions;
  WallType type;
  sf::Texture texture;
  const DifficultySettings* difficultySettings;

  // Timing
  PausableClock lifetimeClock;
  float lifetime;  // Random between 10-20 seconds, adjusted by difficulty
  bool expired;

  // Wall phases
  WallPhase currentPhase;
  static constexpr float APPEARANCE_DURATION = 3.0f;     // 3 seconds to appear
  static constexpr float DISAPPEARANCE_DURATION = 3.0f;  // 3 seconds to disappear

  // Blinking effect
  bool blinking;
  int blinkCount;
  static constexpr int MAX_BLINKS = 3;
  static constexpr float BLINK_DURATION = 0.5f;
  sf::Clock blinkTimer;

  void loadTexture();
  void startBlinking();
  sf::Color getBlinkColor() const;
};
