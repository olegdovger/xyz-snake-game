#include "ResourceLoader.hpp"
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <iostream>


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
    "wall_4",        // TextureType::Wall_4
    "game_ui",       // TextureType::GameUI
    "digits",        // TextureType::Digits
    "game_icon"      // TextureType::GameIcon
};

const std::string MUSIC_NAMES[] = {
    "background_music"  // MusicType::BackgroundMusic
};

const std::string SOUND_NAMES[] = {
    "eat_apple",             // SoundType::EatApple
    "game_over",             // SoundType::GameOver
    "countdown",             // SoundType::Countdown
    "select_menu_item",      // SoundType::SelectMenuItem
    "set_active_menu_item",  // SoundType::SetActiveMenuItem
    "start_game"             // SoundType::StartGame
};

bool ResourceLoader::initializeAllResources() {
  std::cout << "Loading all game resources..." << std::endl;

  bool success = true;
  success &= loadTextures();
  success &= loadFonts();
  success &= loadSounds();
  success &= loadMusic();

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
  success &= loadTexture(textureTypeToString(TextureType::Snake), "resources/Snake.png");
  success &= loadTexture(textureTypeToString(TextureType::GreenApple), "resources/GreenApple.png");
  success &= loadTexture(textureTypeToString(TextureType::RedApple), "resources/RedApple.png");
  success &= loadTexture(textureTypeToString(TextureType::FantomApple), "resources/FantomApple.png");
  success &= loadTexture(textureTypeToString(TextureType::BoardBorder), "resources/BoardBorder.png");
  success &= loadTexture(textureTypeToString(TextureType::BoardGrid), "resources/BoardGrid.png");
  success &= loadTexture(textureTypeToString(TextureType::Portal), "resources/Portal.png");
  success &= loadTexture(textureTypeToString(TextureType::WaterBubble), "resources/WaterBubble.png");
  success &= loadTexture(textureTypeToString(TextureType::Wall_1), "resources/Wall_1.png");
  success &= loadTexture(textureTypeToString(TextureType::Wall_2), "resources/Wall_2.png");
  success &= loadTexture(textureTypeToString(TextureType::Wall_3), "resources/Wall_3.png");
  success &= loadTexture(textureTypeToString(TextureType::Wall_4), "resources/Wall_4.png");

  success &= loadTexture(textureTypeToString(TextureType::GameUI), "resources/GameUI.png");
  success &= loadTexture(textureTypeToString(TextureType::Digits), "resources/Digits.png");
  success &= loadTexture(textureTypeToString(TextureType::GameIcon), "resources/GameIcon.png");

  return success;
}

bool ResourceLoader::loadFonts() {
  std::cout << "Loading fonts..." << std::endl;

  bool success = true;
  success &= loadFont(fontTypeToString(FontType::DebugFont),
                      "resources/fonts/JetBrainsMono"
                      "/fonts/ttf/JetBrainsMono-Regular.ttf");
  success &= loadFont(fontTypeToString(FontType::UIFont), "resources/fonts/Jersey_10/Jersey10-Regular.ttf");

  // Ensure the loaded fonts include Unicode characters for Cyrillic text
  if (success) {
    auto& debugFont = getFontManager().getResource(fontTypeToString(FontType::DebugFont));
    auto& uiFont = getFontManager().getResource(fontTypeToString(FontType::UIFont));

    // Load a basic set of Cyrillic characters to ensure they're included
    // This is a workaround for SFML not loading all characters by default
    std::wstring cyrillicChars = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя";

    sf::Text tempText1(debugFont);
    tempText1.setString(cyrillicChars);

    // Force SFML to generate the glyph textures for Cyrillic characters
    for (wchar_t ch : cyrillicChars) {
      (void)debugFont.getGlyph(ch, 24, false);
      (void)uiFont.getGlyph(ch, 24, false);
    }

    sf::Text tempText2(uiFont);
    tempText2.setString(cyrillicChars);
  }

  return success;
}

bool ResourceLoader::loadSounds() {
  std::cout << "Loading sounds..." << std::endl;

  bool success = true;
  success &= loadSound(soundTypeToString(SoundType::EatApple), "resources/sound/eat_apple.mp3");
  success &= loadSound(soundTypeToString(SoundType::GameOver), "resources/sound/game_over.mp3");
  success &= loadSound(soundTypeToString(SoundType::Countdown), "resources/sound/countdown.mp3");
  success &= loadSound(soundTypeToString(SoundType::SelectMenuItem), "resources/sound/select_menu_item.mp3");
  success &= loadSound(soundTypeToString(SoundType::SetActiveMenuItem), "resources/sound/set_active_menu_item.mp3");
  success &= loadSound(soundTypeToString(SoundType::StartGame), "resources/sound/start_game.mp3");

  return success;
}

bool ResourceLoader::loadMusic() {
  std::cout << "Loading music..." << std::endl;

  bool success = true;
  success &= loadMusicFile(musicTypeToString(MusicType::BackgroundMusic), "resources/sound/background_music.mp3");

  return success;
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

bool ResourceLoader::loadMusicFile(const std::string& name, const std::string& path) {
  return getMusicManager().loadResource(name, path);
}

ResourceManager<sf::Texture>& ResourceLoader::getTextureManager() {
  return TextureManager::getInstance();
}

FontManager& ResourceLoader::getFontManager() {
  return FontManager::getInstance();
}

SoundBufferManager& ResourceLoader::getSoundManager() {
  return SoundBufferManager::getInstance();
}

MusicManager& ResourceLoader::getMusicManager() {
  return MusicManager::getInstance();
}

std::string ResourceLoader::fontTypeToString(const FontType fontType) {
  return FONT_NAMES[static_cast<int>(fontType)];
}

std::string ResourceLoader::textureTypeToString(const TextureType textureType) {
  return TEXTURE_NAMES[static_cast<int>(textureType)];
}

std::string ResourceLoader::musicTypeToString(const MusicType musicType) {
  return MUSIC_NAMES[static_cast<int>(musicType)];
}

std::string ResourceLoader::soundTypeToString(const SoundType soundType) {
  return SOUND_NAMES[static_cast<int>(soundType)];
}

const sf::Font& ResourceLoader::getFont(const FontType fontType) {
  return getFontManager().getResource(fontTypeToString(fontType));
}

const sf::Texture& ResourceLoader::getTexture(const TextureType textureType) {
  return getTextureManager().getResource(textureTypeToString(textureType));
}

sf::Music& ResourceLoader::getMusic(const MusicType musicType) {
  return getMusicManager().getResource(musicTypeToString(musicType));
}

const sf::SoundBuffer& ResourceLoader::getSound(const SoundType soundType) {
  return getSoundManager().getResource(soundTypeToString(soundType));
}
