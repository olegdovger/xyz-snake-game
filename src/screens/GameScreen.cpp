#include <iostream>
#include "../Screen.hpp"
#include "../utils/ResourceLoader.hpp"

class GameScreen : public Screen {
public:
  explicit GameScreen(sf::RenderWindow& win, Game& gameRef)
      : Screen(win, gameRef) {}

  void processEvents(const sf::Event& event) override {
    std::cout << "GameScreen screen - Handling game events" << std::endl;
  }

  void update() override {
    std::cout << "GameScreen screen - Updating game logic" << std::endl;
  }

  float rescaleSprite(sf::Sprite& sprite, float scaleRelativeFactor = 1.0f) {
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = sprite.getTexture().getSize();
    float scaleX = static_cast<float>(windowSize.x - windowSize.x * 0.1f) /
                   static_cast<float>(textureSize.x);
    float scaleY = static_cast<float>(windowSize.y - windowSize.y * 0.1f) /
                   static_cast<float>(textureSize.y);
    float scale = std::min(scaleX, scaleY);  // 80% of window size
    sprite.setScale(
        sf::Vector2f(scale * scaleRelativeFactor, scale * scaleRelativeFactor));

    return scale;
  }

  void setSpritePositionToCenter(sf::Sprite& sprite, float scale) {
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = sprite.getTexture().getSize();

    // Recalculate texture size after scaling
    sf::Vector2f scaledSize(static_cast<float>(textureSize.x) * scale,
                            static_cast<float>(textureSize.y) * scale);

    // Center the scaled sprite
    sprite.setPosition(sf::Vector2f((windowSize.x - scaledSize.x) / 2.0f,
                                    (windowSize.y - scaledSize.y) / 2.0f));
  }

  void renderBoard() {
    // Then render border on top
    renderBoardBorder();
    // Render grid first (background)
    renderBoardGrid();
  }

  void renderBoardBorder() {
    sf::Texture texture =
        utils::ResourceLoader::getTexture(utils::TextureType::BoardBorder);
    sf::Sprite sprite(texture);

    float scale = rescaleSprite(sprite);
    setSpritePositionToCenter(sprite, scale);

    std::cout << "Drawing BoardBorder at scale: " << scale
              << ", texture size: " << texture.getSize().x << "x"
              << texture.getSize().y << std::endl;

    // sprite.setColor(sf::Color(255, 255, 255, 48));

    window.draw(sprite);
  }

  void renderBoardGrid() {
    sf::Texture texture =
        utils::ResourceLoader::getTexture(utils::TextureType::BoardGrid);
    sf::Sprite sprite(texture);

    float scaleRelativeFactor = 912.0f / 992.0f;
    float scale = rescaleSprite(sprite, scaleRelativeFactor);
    setSpritePositionToCenter(sprite, scale * scaleRelativeFactor);

    std::cout << "Drawing BoardGrid at scale: " << scale
              << ", texture size: " << texture.getSize().x << "x"
              << texture.getSize().y << std::endl;

    // sprite.setColor(sf::Color(255, 255, 255, 128));

    window.draw(sprite);
  }

  void render() override {
    std::cout << "GameScreen screen - Game is running!" << std::endl;

    renderBoard();
  }
};
