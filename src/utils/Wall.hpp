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

  void update(float deltaTime);
  bool isExpired() const { return expired; }
  bool isBlinking() const { return blinking; }

  void render(sf::RenderWindow& window, const GameGrid& grid) const;

  const std::vector<sf::Vector2i>& getPositions() const { return positions; }
  WallType getType() const { return type; }

  bool checkCollisionWithPosition(sf::Vector2i position) const;
  bool canCollide() const;
  WallPhase getCurrentPhase() const { return currentPhase; }

private:
  std::vector<sf::Vector2i> positions;
  WallType type;
  sf::Texture texture;
  const DifficultySettings* difficultySettings;

  PausableClock lifetimeClock;
  float lifetime;
  bool expired;

  WallPhase currentPhase;
  static constexpr float APPEARANCE_DURATION = 3.0f;
  static constexpr float DISAPPEARANCE_DURATION = 3.0f;

  bool blinking;
  int blinkCount;
  static constexpr int MAX_BLINKS = 3;
  static constexpr float BLINK_DURATION = 0.5f;
  sf::Clock blinkTimer;

  void loadTexture();
  void startBlinking();
  sf::Color getBlinkColor() const;
};
