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

  // Load resource from file
  bool loadResource(const std::string& name, const std::string& filePath);

  // Get resource by name
  T& getResource(const std::string& name);
  const T& getResource(const std::string& name) const;

  // Check if resource exists
  bool hasResource(const std::string& name) const;

  // Unload resource
  void unloadResource(const std::string& name);

  // Unload all resources
  void clear();

  // Get resource count
  size_t getResourceCount() const;

private:
  ResourceManager() = default;
  ~ResourceManager() = default;
  ResourceManager(const ResourceManager&) = delete;
  ResourceManager& operator=(const ResourceManager&) = delete;

  std::unordered_map<std::string, std::unique_ptr<T>> resources;
};

// Specialized managers for common SFML types
using TextureManager = ResourceManager<sf::Texture>;
using FontManager = ResourceManager<sf::Font>;
using SoundBufferManager = ResourceManager<sf::SoundBuffer>;
