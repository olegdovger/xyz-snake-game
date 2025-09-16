#include "GameScreen.hpp"
#include "PauseScreen.hpp"

GameScreen::GameScreen(sf::RenderWindow& win, Game& gameRef) : Screen(win, gameRef) {}

void GameScreen::processEvents(const sf::Event& event) {
  std::cout << "GameScreen screen - Handling game events" << std::endl;
  if (event.is<sf::Event::KeyPressed>()) {
    switch (event.getIf<sf::Event::KeyPressed>()->code) {
      case sf::Keyboard::Key::Escape:
      case sf::Keyboard::Key::P:
        game.setCurrentScreen(new PauseScreen(window, game));
        break;
      default:
        break;
    }
  }
}

void GameScreen::update() {
  std::cout << "GameScreen screen - Updating game logic" << std::endl;
}

float GameScreen::rescaleSprite(sf::Sprite& sprite, const float scaleRelativeFactor) const {
  const sf::Vector2u windowSize = window.getSize();
  const sf::Vector2u textureSize = sprite.getTexture().getSize();
  const float scaleX = (float)(windowSize.x * 0.9f) / (float)(textureSize.x);
  const float scaleY = (float)(windowSize.y * 0.9f) / (float)(textureSize.y);
  const float scale = std::min(scaleX, scaleY);  // 80% of window size
  sprite.setScale(sf::Vector2f(scale * scaleRelativeFactor, scale * scaleRelativeFactor));

  return scale;
}

void GameScreen::setSpritePositionToCenter(sf::Sprite& sprite, float scale) const {
  const sf::Vector2u windowSize = window.getSize();
  const sf::Vector2u textureSize = sprite.getTexture().getSize();

  // Recalculate texture size after scaling
  const sf::Vector2f scaledSize(static_cast<float>(textureSize.x) * scale, static_cast<float>(textureSize.y) * scale);

  // Center the scaled sprite
  sprite.setPosition(sf::Vector2f((windowSize.x - scaledSize.x) / 2.0f, (windowSize.y - scaledSize.y) / 2.0f));
}

void GameScreen::renderBoard() {
  // Then render border on top
  renderBoardBorder();
  // Render grid first (background)
  renderBoardGrid();
}

void GameScreen::renderBoardBorder() {
  sf::Texture texture = utils::ResourceLoader::getTexture(utils::TextureType::BoardBorder);
  sf::Sprite sprite(texture);

  const float scale = rescaleSprite(sprite);
  setSpritePositionToCenter(sprite, scale);

  // sprite.setColor(sf::Color(255, 255, 255, 48));

  window.draw(sprite);
}

void GameScreen::renderBoardGrid() {
  sf::Texture texture = utils::ResourceLoader::getTexture(utils::TextureType::BoardGrid);
  sf::Sprite sprite(texture);

  const float scaleRelativeFactor = 912.0f / 992.0f;
  const float scale = rescaleSprite(sprite, scaleRelativeFactor);
  setSpritePositionToCenter(sprite, scale * scaleRelativeFactor);

  std::cout << "Drawing BoardGrid at scale: " << scale << ", texture size: " << texture.getSize().x << "x"
            << texture.getSize().y << std::endl;

  // sprite.setColor(sf::Color(255, 255, 255, 128));

  window.draw(sprite);
}

void GameScreen::render() {
  std::cout << "GameScreen screen - Game is running!" << std::endl;

  renderBoard();
}
