#include "ResourceLoader.hpp"
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <iostream>
#include <map>

const std::map<FontType, std::string> FONT_NAMES = {{FontType::DebugFont, "debug_font"}, {FontType::UIFont, "ui_font"}};
const std::map<TextureType, std::string> TEXTURE_NAMES = {{TextureType::Snake, "snake"},
                                                          {TextureType::GreenApple, "green_apple"},
                                                          {TextureType::RedApple, "red_apple"},
                                                          {TextureType::FantomApple, "fantom_apple"},
                                                          {TextureType::BoardBorder, "board_border"},
                                                          {TextureType::BoardGrid, "board_grid"},
                                                          {TextureType::Portal, "portal"},
                                                          {TextureType::WaterBubble, "water_bubble"},
                                                          {TextureType::Wall_1, "wall_1"},
                                                          {TextureType::Wall_2, "wall_2"},
                                                          {TextureType::Wall_3, "wall_3"},
                                                          {TextureType::Wall_4, "wall_4"},
                                                          {TextureType::GameUI, "game_ui"},
                                                          {TextureType::Digits, "digits"},
                                                          {TextureType::GameIcon, "game_icon"}};
const std::map<MusicType, std::string> MUSIC_NAMES = {{MusicType::BackgroundMusic, "background_music"}};
const std::map<SoundType, std::string> SOUND_NAMES = {{SoundType::EatApple, "eat_apple"},
                                                      {SoundType::GameOver, "game_over"},
                                                      {SoundType::Countdown, "countdown"},
                                                      {SoundType::SelectMenuItem, "select_menu_item"},
                                                      {SoundType::SetActiveMenuItem, "set_active_menu_item"},
                                                      {SoundType::StartGame, "start_game"}};

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
  return FONT_NAMES.at(fontType);
}

std::string ResourceLoader::textureTypeToString(const TextureType textureType) {
  return TEXTURE_NAMES.at(textureType);
}

std::string ResourceLoader::musicTypeToString(const MusicType musicType) {
  return MUSIC_NAMES.at(musicType);
}

std::string ResourceLoader::soundTypeToString(const SoundType soundType) {
  return SOUND_NAMES.at(soundType);
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
