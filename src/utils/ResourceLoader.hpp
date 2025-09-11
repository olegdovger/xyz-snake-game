#pragma once
#include <string>
#include "ResourceManager.hpp"

namespace utils {

enum class FontType { DebugFont, UIFont };

enum class TextureType {
  Snake,
  GreenApple,
  RedApple,
  FantomApple,
  BoardBorder,
  BoardGrid,
  Portal,
  WaterBubble,
  Wall_1,
  Wall_2,
  Wall_3,
  Wall_4
};

class ResourceLoader {
public:
  // Initialize all game resources
  static bool initializeAllResources();

  // Load specific resource types
  static bool loadTextures();

  static bool loadFonts();

  static bool loadSounds();

  // Get resource managers
  static TextureManager& getTextureManager();

  static FontManager& getFontManager();

  static SoundBufferManager& getSoundManager();

  // Get font by type (type-safe)
  static const sf::Font& getFont(FontType fontType);

  static const sf::Texture& getTexture(TextureType textureType);

private:
  static bool loadTexture(const std::string& name, const std::string& path);

  static bool loadFont(const std::string& name, const std::string& path);

  static bool loadSound(const std::string& name, const std::string& path);

  // Convert enum to string
  static std::string fontTypeToString(FontType fontType);

  static std::string textureTypeToString(TextureType textureType);
};

} // namespace utils
