#include "ResourceManager.hpp"
#include <iostream>
#include <type_traits>

namespace utils {

template <typename T>
ResourceManager<T>& ResourceManager<T>::getInstance() {
  static ResourceManager<T> instance;
  return instance;
}

template <typename T>
bool ResourceManager<T>::loadResource(const std::string& name,
                                      const std::string& filePath) {
  // Check if resource already exists
  if (hasResource(name)) {
    std::cout << "Resource '" << name << "' already loaded" << std::endl;
    return true;
  }

  // Create new resource
  auto resource = std::make_unique<T>();

  // Try to load the resource
  if constexpr (std::is_same_v<T, sf::Font>) {
    if (!resource->openFromFile(filePath)) {
      std::cerr << "Failed to load resource '" << name << "' from '" << filePath
                << "'" << std::endl;
      return false;
    }
  } else if constexpr (std::is_same_v<T, sf::Texture>) {
    if (!resource->loadFromFile(filePath)) {
      std::cerr << "Failed to load resource '" << name << "' from '" << filePath
                << "'" << std::endl;
      return false;
    }
  } else if constexpr (std::is_same_v<T, sf::SoundBuffer>) {
    if (!resource->loadFromFile(filePath)) {
      std::cerr << "Failed to load resource '" << name << "' from '" << filePath
                << "'" << std::endl;
      return false;
    }
  }

  // Store the resource
  resources[name] = std::move(resource);
  std::cout << "Successfully loaded resource '" << name << "' from '"
            << filePath << "'" << std::endl;
  return true;
}

template <typename T>
T& ResourceManager<T>::getResource(const std::string& name) {
  auto it = resources.find(name);
  if (it == resources.end()) {
    std::cerr << "Resource '" << name << "' not found!" << std::endl;
    assert(false && "Resource not found - use assert for critical resources");
  }
  return *it->second;
}

template <typename T>
const T& ResourceManager<T>::getResource(const std::string& name) const {
  auto it = resources.find(name);
  if (it == resources.end()) {
    std::cerr << "Resource '" << name << "' not found!" << std::endl;
    assert(false && "Resource not found - use assert for critical resources");
  }
  return *it->second;
}

template <typename T>
bool ResourceManager<T>::hasResource(const std::string& name) const {
  return resources.find(name) != resources.end();
}

template <typename T>
void ResourceManager<T>::unloadResource(const std::string& name) {
  auto it = resources.find(name);
  if (it != resources.end()) {
    resources.erase(it);
    std::cout << "Unloaded resource '" << name << "'" << std::endl;
  }
}

template <typename T>
void ResourceManager<T>::clear() {
  resources.clear();
  std::cout << "Cleared all resources" << std::endl;
}

template <typename T>
size_t ResourceManager<T>::getResourceCount() const {
  return resources.size();
}

// Explicit template instantiations for common types
template class ResourceManager<sf::Texture>;
template class ResourceManager<sf::Font>;
template class ResourceManager<sf::SoundBuffer>;

}  // namespace utils
