#include "Settings.hpp"
#include <iostream>
#include "../utils/FontInitializer.hpp"
#include "../utils/ScalingUtils.hpp"
#include "../utils/SettingStorage.hpp"
#include "MainMenu.hpp"

using namespace shape;

Settings::Settings(sf::RenderWindow& win, Game& gameRef) : Screen(win, gameRef), titleText(font), backText(font) {
  font = FontInitializer::getDebugFont();
  FontInitializer::initializeTitleText(titleText, font, L"Настройки");
  FontInitializer::initializeBackText(backText, font, 14);

  screenRect.setSize(originSize);
  screenRect.setFillColor(menuBackgroundColor);
  screenRect.setOutlineColor(borderColor);
  screenRect.setOutlineThickness(10.0f);

  loadSettings();
  soundManager.setSoundEnabled(soundEnabled);
  initializeMenuItems();
}

void Settings::processEvents(const sf::Event& event) {
  if (event.is<sf::Event::KeyPressed>()) {
    switch (event.getIf<sf::Event::KeyPressed>()->code) {
      case sf::Keyboard::Key::W:
      case sf::Keyboard::Key::Up:
        std::cout << "Keypressed up(w)" << std::endl;
        selectedIndex = (selectedIndex - 1 + MENU_ITEMS_COUNT) % MENU_ITEMS_COUNT;
        soundManager.playNavigationSound();
        break;
      case sf::Keyboard::Key::S:
      case sf::Keyboard::Key::Down:
        std::cout << "Keypressed down(s)" << std::endl;
        selectedIndex = (selectedIndex + 1) % MENU_ITEMS_COUNT;
        soundManager.playNavigationSound();
        break;
      case sf::Keyboard::Key::Enter:
        soundManager.playSelectionSound();
        toggleSoundSetting();
        break;
      case sf::Keyboard::Key::Escape:
      case sf::Keyboard::Key::B:
        game.setCurrentScreen(new MainMenu(window, game));
        break;
      default:
        break;
    }
  }
}

void Settings::update() {}

void Settings::render() {
  renderMenuRect();
  renderTitle();
  renderMenuItems();
  renderBackButton();
}

void Settings::initializeMenuItems() {
  menuLabels = {std::wstring(L"Звук: ") + (soundEnabled ? L"Включен" : L"Выключен"),
                std::wstring(L"Музыка: ") + (musicEnabled ? L"Включена" : L"Выключена")};

  menuItems.clear();
  menuItems.reserve(menuLabels.size());

  for (size_t i = 0; i < menuLabels.size(); ++i) {
    sf::Text item(font);
    FontInitializer::initializeMenuItemText(item, font, menuLabels[i], 24);
    menuItems.push_back(item);
  }
}

void Settings::renderMenuRect() {
  sf::Vector2u windowSize = window.getSize();
  screenRect.setPosition(sf::Vector2f((static_cast<float>(windowSize.x) - screenRect.getSize().x) / 2.0f,
                                      (static_cast<float>(windowSize.y) - screenRect.getSize().y) / 2.0f));

  const float scale = getScale(sf::Vector2f(screenRect.getSize()), window.getSize()) * 0.8f;
  screenRect.setScale(sf::Vector2f(scale, scale));

  const auto position = getPosition(sf::Vector2f(screenRect.getSize()), window.getSize(), scale);
  screenRect.setPosition(position);

  window.draw(screenRect);
}

void Settings::renderTitle() {
  const auto position =
      getPosition(sf::Vector2f(titleText.getLocalBounds().size), window.getSize(), screenRect.getScale().x);
  titleText.setPosition(sf::Vector2f(position.x, screenRect.getPosition().y + 20 * screenRect.getScale().y));

  titleText.setScale(screenRect.getScale());

  window.draw(titleText);
}

void Settings::renderMenuItems() {
  for (size_t i = 0; i < menuItems.size(); ++i) {
    sf::Text item = menuItems[i];

    const auto position =
        getPosition(sf::Vector2f(item.getLocalBounds().size), window.getSize(), screenRect.getScale().x);

    item.setPosition(sf::Vector2f(position.x, screenRect.getPosition().y + 100.0f * screenRect.getScale().y +
                                                  i * 50.0f * screenRect.getScale().y));

    item.setScale(screenRect.getScale());

    if (i == selectedIndex) {
      item.setFillColor(textColor);
      item.setStyle(sf::Text::Underlined);
    } else {
      item.setFillColor(sf::Color::White);
      item.setStyle(sf::Text::Regular);
    }

    window.draw(item);
  }
}

void Settings::toggleSoundSetting() {
  SettingStorage settingStorage;
  settingStorage.loadSettings();

  switch (selectedIndex) {
    case 0:
      soundEnabled = !soundEnabled;
      settingStorage.setGameSound(soundEnabled);
      break;
    case 1:
      musicEnabled = !musicEnabled;
      settingStorage.setGameMusic(musicEnabled);
      break;
  }

  settingStorage.saveSettings();
  initializeMenuItems();
}

void Settings::renderBackButton() {
  backText.setScale(screenRect.getScale());

  const auto position =
      getPosition(sf::Vector2f(backText.getLocalBounds().size), window.getSize(), screenRect.getScale().x);
  backText.setPosition(sf::Vector2f(
      position.x,
      screenRect.getPosition().y + screenRect.getSize().y * screenRect.getScale().y - 40.0f * screenRect.getScale().y));
  window.draw(backText);
}

void Settings::loadSettings() {
  SettingStorage settingStorage;

  bool settingsLoaded = settingStorage.loadSettings();

  if (settingsLoaded) {
    soundEnabled = settingStorage.getGameSound();
    musicEnabled = settingStorage.getGameMusic();
  } else {
    soundEnabled = true;
    musicEnabled = true;
  }
}
