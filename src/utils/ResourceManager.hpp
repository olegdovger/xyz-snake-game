#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>

template <typename T>
class ResourceManager {
public:
  static ResourceManager& getInstance();

  bool loadResource(const std::string& name, const std::string& filePath);

  T& getResource(const std::string& name);
  const T& getResource(const std::string& name) const;

  bool hasResource(const std::string& name) const;

  void unloadResource(const std::string& name);

  void clear();

  size_t getResourceCount() const;

private:
  ResourceManager() = default;
  ~ResourceManager() = default;
  ResourceManager(const ResourceManager&) = delete;
  ResourceManager& operator=(const ResourceManager&) = delete;

  std::unordered_map<std::string, std::unique_ptr<T>> resources;
};

using TextureManager = ResourceManager<sf::Texture>;
using FontManager = ResourceManager<sf::Font>;
using SoundBufferManager = ResourceManager<sf::SoundBuffer>;
using MusicManager = ResourceManager<sf::Music>;