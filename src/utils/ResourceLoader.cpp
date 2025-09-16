#include "ResourceLoader.hpp"
#include <iostream>

namespace utils {

// Font name mapping
namespace {
const std::string FONT_NAMES[] = {
    "debug_font",  // FontType::DebugFont
    "ui_font"      // FontType::UIFont
};
const std::string TEXTURE_NAMES[] = {
    "snake",         // TextureType::Snake
    "green_apple",   // TextureType::GreenApple
    "red_apple",     // TextureType::RedApple
    "fantom_apple",  // TextureType::FantomApple
    "board_border",  // TextureType::BoardBorder
    "board_grid",    // TextureType::BoardGrid
    "portal",        // TextureType::Portal
    "water_bubble",  // TextureType::WaterBubble
    "wall_1",        // TextureType::Wall_1
    "wall_2",        // TextureType::Wall_2
    "wall_3",        // TextureType::Wall_3
    "wall_4"         // TextureType::Wall_4
};
}  // namespace

bool ResourceLoader::initializeAllResources() {
  std::cout << "Loading all game resources..." << std::endl;

  bool success = true;
  success &= loadTextures();
  success &= loadFonts();
  success &= loadSounds();

  if (success) {
    std::cout << "All resources loaded successfully!" << std::endl;
  } else {
    std::cerr << "Some resources failed to load!" << std::endl;
  }

  return success;
}

bool ResourceLoader::loadTextures() {
  std::cout << "Loading textures..." << std::endl;

  bool success = true;
  success &= loadTexture(textureTypeToString(TextureType::Snake), "../../resources/Snake.png");
  success &= loadTexture(textureTypeToString(TextureType::GreenApple), "../../resources/GreenApple.png");
  success &= loadTexture(textureTypeToString(TextureType::RedApple), "../../resources/RedApple.png");
  success &= loadTexture(textureTypeToString(TextureType::FantomApple), "../../resources/FantomApple.png");
  success &= loadTexture(textureTypeToString(TextureType::BoardBorder), "../../resources/BoardBorder.png");
  success &= loadTexture(textureTypeToString(TextureType::BoardGrid), "../../resources/BoardGrid.png");
  success &= loadTexture(textureTypeToString(TextureType::Portal), "../../resources/Portal.png");
  success &= loadTexture(textureTypeToString(TextureType::WaterBubble), "../../resources/WaterBubble.png");
  success &= loadTexture(textureTypeToString(TextureType::Wall_1), "../../resources/Wall_1.png");
  success &= loadTexture(textureTypeToString(TextureType::Wall_2), "../../resources/Wall_2.png");
  success &= loadTexture(textureTypeToString(TextureType::Wall_3), "../../resources/Wall_3.png");
  success &= loadTexture(textureTypeToString(TextureType::Wall_4), "../../resources/Wall_4.png");

  return success;
}

bool ResourceLoader::loadFonts() {
  std::cout << "Loading fonts..." << std::endl;

  bool success = true;
  success &= loadFont(fontTypeToString(FontType::DebugFont),
                      "../../resources/fonts/JetBrainsMono"
                      "/fonts/ttf/JetBrainsMono-Regular.ttf");
  success &= loadFont(fontTypeToString(FontType::UIFont), "../../resources/fonts/Jersey_10/Jersey10-Regular.ttf");

  return success;
}

bool ResourceLoader::loadSounds() {
  std::cout << "Loading sounds..." << std::endl;

  // Add sound files here when available
  // success &= loadSound("eat_apple", "../../resources/sounds/eat_apple.wav");
  // success &= loadSound("game_over", "../../resources/sounds/game_over.wav");

  return true;  // No sounds yet
}

bool ResourceLoader::loadTexture(const std::string& name, const std::string& path) {
  return getTextureManager().loadResource(name, path);
}

bool ResourceLoader::loadFont(const std::string& name, const std::string& path) {
  return getFontManager().loadResource(name, path);
}

bool ResourceLoader::loadSound(const std::string& name, const std::string& path) {
  return getSoundManager().loadResource(name, path);
}

TextureManager& ResourceLoader::getTextureManager() {
  return TextureManager::getInstance();
}

FontManager& ResourceLoader::getFontManager() {
  return FontManager::getInstance();
}

SoundBufferManager& ResourceLoader::getSoundManager() {
  return SoundBufferManager::getInstance();
}

std::string ResourceLoader::fontTypeToString(const FontType fontType) {
  return FONT_NAMES[static_cast<int>(fontType)];
}

std::string ResourceLoader::textureTypeToString(const TextureType textureType) {
  return TEXTURE_NAMES[static_cast<int>(textureType)];
}

const sf::Font& ResourceLoader::getFont(const FontType fontType) {
  return getFontManager().getResource(fontTypeToString(fontType));
}

const sf::Texture& ResourceLoader::getTexture(const TextureType textureType) {
  return getTextureManager().getResource(textureTypeToString(textureType));
}
}  // namespace utils
