#pragma once

#include <SFML/Audio.hpp>

class MenuSoundManager {
private:
  sf::Sound setActiveMenuItemSound;
  sf::Sound selectMenuItemSound;
  bool soundEnabled = true;

public:
  MenuSoundManager();
  ~MenuSoundManager() = default;

  void setSoundEnabled(bool enabled);
  void playNavigationSound();
  void playSelectionSound();

  [[nodiscard]] bool isSoundEnabled() const { return soundEnabled; }
};
