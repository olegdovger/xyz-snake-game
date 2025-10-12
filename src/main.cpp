#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "utils/ResourceLoader.hpp"

int main() {
  sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Snake Game");

  // Initialize resources
  ResourceLoader::initializeAllResources();

  // Set application icon using ResourceLoader
  const sf::Texture& iconTexture = ResourceLoader::getTexture(TextureType::GameIcon);
  sf::Image icon = iconTexture.copyToImage();

  window.setIcon(icon.getSize(), icon.getPixelsPtr());

  const Game game(window);

  game.start();

  return 0;
}