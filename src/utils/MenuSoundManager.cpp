#include "MenuSoundManager.hpp"
#include "../config/AudioConstants.hpp"
#include "ResourceLoader.hpp"

MenuSoundManager::MenuSoundManager()
    : setActiveMenuItemSound(ResourceLoader::getSound(SoundType::SetActiveMenuItem)),
      selectMenuItemSound(ResourceLoader::getSound(SoundType::SelectMenuItem)) {

  setActiveMenuItemSound.setVolume(AudioConstants::SoundEffects::MENU_NAVIGATION_VOLUME);
  selectMenuItemSound.setVolume(AudioConstants::SoundEffects::MENU_SELECTION_VOLUME);
}

void MenuSoundManager::setSoundEnabled(bool enabled) {
  soundEnabled = enabled;
}

void MenuSoundManager::playNavigationSound() {
  if (soundEnabled) {
    setActiveMenuItemSound.play();
  }
}

void MenuSoundManager::playSelectionSound() {
  if (soundEnabled) {
    selectMenuItemSound.play();
  }
}
