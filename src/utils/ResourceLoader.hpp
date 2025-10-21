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
  GameIcon
};

enum class MusicType { BackgroundMusic };

enum class SoundType { EatApple, GameOver, Countdown, SelectMenuItem, SetActiveMenuItem, StartGame };

class ResourceLoader {
public:
  static bool initializeAllResources();

  static bool loadTextures();

  static bool loadFonts();

  static bool loadSounds();

  static bool loadMusic();

  static TextureManager& getTextureManager();

  static FontManager& getFontManager();

  static SoundBufferManager& getSoundManager();

  static MusicManager& getMusicManager();

  static const sf::Font& getFont(const FontType fontType);

  static const sf::Texture& getTexture(const TextureType textureType);

  static sf::Music& getMusic(const MusicType musicType);

  static const sf::SoundBuffer& getSound(const SoundType soundType);

private:
  static bool loadTexture(const std::string& name, const std::string& path);

  static bool loadFont(const std::string& name, const std::string& path);

  static bool loadSound(const std::string& name, const std::string& path);

  static bool loadMusicFile(const std::string& name, const std::string& path);

  static std::string fontTypeToString(const FontType fontType);

  static std::string textureTypeToString(const TextureType textureType);

  static std::string musicTypeToString(const MusicType musicType);

  static std::string soundTypeToString(const SoundType soundType);
};
