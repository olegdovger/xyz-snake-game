#pragma once
#include <string>
#include "ResourceManager.hpp"

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
  Wall_4,
  GameUI,
  Digits,
};

enum class MusicType {
  BackgroundMusic
};

class ResourceLoader {
public:
  // Initialize all game resources
  static bool initializeAllResources();

  // Load specific resource types
  static bool loadTextures();

  static bool loadFonts();

  static bool loadSounds();

  static bool loadMusic();

  // Get resource managers
  static TextureManager& getTextureManager();

  static FontManager& getFontManager();

  static SoundBufferManager& getSoundManager();

  static MusicManager& getMusicManager();

  // Get font by type (type-safe)
  static const sf::Font& getFont(const FontType fontType);

  static const sf::Texture& getTexture(const TextureType textureType);

  static sf::Music& getMusic(const MusicType musicType);

private:
  static bool loadTexture(const std::string& name, const std::string& path);

  static bool loadFont(const std::string& name, const std::string& path);

  static bool loadSound(const std::string& name, const std::string& path);

  static bool loadMusicFile(const std::string& name, const std::string& path);

  // Convert enum to string
  static std::string fontTypeToString(const FontType fontType);

  static std::string textureTypeToString(const TextureType textureType);

  static std::string musicTypeToString(const MusicType musicType);
};
