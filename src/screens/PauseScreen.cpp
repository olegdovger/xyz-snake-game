#include "PauseScreen.hpp"
#include <iostream>
#include "../utils/ResourceLoader.hpp"
#include "../utils/ScalingUtils.hpp"
#include "../utils/SettingStorage.hpp"
#include "Exit.hpp"
#include "MainMenu.hpp"

using namespace shape;

PauseScreen::PauseScreen(sf::RenderWindow& win, Game& gameRef)
    : Screen(win, gameRef),
      titleText(font),
      backText(font),
      setActiveMenuItemSound(ResourceLoader::getSound(SoundType::SetActiveMenuItem)),
      selectMenuItemSound(ResourceLoader::getSound(SoundType::SelectMenuItem)) {
  font = ResourceLoader::getFont(FontType::DebugFont);

  titleText.setString(L"Пауза");
  titleText.setFont(font);
  titleText.setCharacterSize(40);
  titleText.setFillColor(sf::Color::White);
  titleText.setStyle(sf::Text::Bold);

  backText.setString(L"Назад (Escape)");
  backText.setFont(font);
  backText.setCharacterSize(14);
  backText.setFillColor(sf::Color::White);
  backText.setStyle(sf::Text::Bold);

  screenRect.setSize(originSize);
  screenRect.setFillColor(menuBackgroundColor);
  screenRect.setOutlineColor(borderColor);
  screenRect.setOutlineThickness(10.0f);

  screenRect.setSize(originSize);
  screenRect.setFillColor(menuBackgroundColor);
  screenRect.setOutlineColor(borderColor);
  screenRect.setOutlineThickness(10.0f);

  // Set menu sound volumes
  setActiveMenuItemSound.setVolume(30.0f);  // 30% volume
  selectMenuItemSound.setVolume(40.0f);     // 40% volume

  // Load sound settings
  SettingStorage settingStorage;
  if (settingStorage.initialize()) {
    soundEnabled = settingStorage.getGameSound();
  }

  initializeMenuItems();
}

void PauseScreen::initializeMenuItems() {
  std::vector<std::wstring> textItems = {L"Продолжить игру", L"Главное меню", L"Выход"};

  menuItems.clear();
  menuItems.reserve(textItems.size());

  for (size_t i = 0; i < textItems.size(); ++i) {
    sf::Text item(font);
    item.setString(textItems[i]);
    item.setCharacterSize(24);
    item.setFillColor(sf::Color::White);

    menuItems.push_back(item);
  }
}

void PauseScreen::processEvents(const sf::Event& event) {
  if (event.is<sf::Event::KeyPressed>()) {
    switch (event.getIf<sf::Event::KeyPressed>()->code) {
      case sf::Keyboard::Key::W:
      case sf::Keyboard::Key::Up:
        std::cout << "Keypressed up(w)" << std::endl;
        selectedIndex = (selectedIndex - 1 + MENU_ITEMS_COUNT) % MENU_ITEMS_COUNT;
        if (soundEnabled) {
          setActiveMenuItemSound.play();  // Play sound when switching menu items
        }
        break;
      case sf::Keyboard::Key::S:
      case sf::Keyboard::Key::Down:
        std::cout << "Keypressed down(s)" << std::endl;
        selectedIndex = (selectedIndex + 1) % MENU_ITEMS_COUNT;
        if (soundEnabled) {
          setActiveMenuItemSound.play();  // Play sound when switching menu items
        }
        break;
      case sf::Keyboard::Key::Enter:
        if (soundEnabled) {
          selectMenuItemSound.play();  // Play sound when selecting menu item
        }
        switch (selectedIndex) {
          case 0:  // Продолжить игру
            // Return to previous screen with countdown restart
            game.returnToGameScreen();
            break;
          case 1:  // Главное меню
            game.setCurrentScreen(new MainMenu(window, game));
            break;
          case 2:  // Выход
            game.setCurrentScreen(new Exit(window, game));
            break;
        }
        break;
      case sf::Keyboard::Key::Escape:
        if (soundEnabled) {
          selectMenuItemSound.play();  // Play sound when selecting menu item
        }
        // Return to previous screen with countdown restart
        game.returnToGameScreen();
        break;
      default:
        break;
    }
  }
}

void PauseScreen::update() {
  // No update logic needed for pause menu
}

void PauseScreen::render() {
  renderMenuRect();
  renderTitle();
  renderMenuItems();
  renderBackButton();
}

void PauseScreen::renderMenuRect() {
  sf::Vector2u windowSize = window.getSize();
  screenRect.setPosition(sf::Vector2f((static_cast<float>(windowSize.x) - screenRect.getSize().x) / 2.0f,
                                      (static_cast<float>(windowSize.y) - screenRect.getSize().y) / 2.0f));

  const float scale = getScale(sf::Vector2f(screenRect.getSize()), window.getSize()) * 0.8f;
  screenRect.setScale(sf::Vector2f(scale, scale));

  const auto position = getPosition(sf::Vector2f(screenRect.getSize()), window.getSize(), scale);
  screenRect.setPosition(position);

  window.draw(screenRect);
}

void PauseScreen::renderTitle() {
  const auto position =
      getPosition(sf::Vector2f(titleText.getLocalBounds().size), window.getSize(), screenRect.getScale().x);
  titleText.setPosition(sf::Vector2f(position.x, screenRect.getPosition().y + 20 * screenRect.getScale().y));

  titleText.setScale(screenRect.getScale());

  window.draw(titleText);
}

void PauseScreen::renderMenuItems() {

  for (size_t i = 0; i < menuItems.size(); ++i) {
    sf::Text item = menuItems[i];

    const auto position =
        getPosition(sf::Vector2f(item.getLocalBounds().size), window.getSize(), screenRect.getScale().x);

    item.setPosition(sf::Vector2f(position.x, screenRect.getPosition().y + 100.0f * screenRect.getScale().y +
                                                  i * 50.0f * screenRect.getScale().y));

    item.setScale(screenRect.getScale());
    window.draw(item);

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

void PauseScreen::renderBackButton() {
  backText.setScale(screenRect.getScale());

  const auto position =
      getPosition(sf::Vector2f(backText.getLocalBounds().size), window.getSize(), screenRect.getScale().x);
  backText.setPosition(sf::Vector2f(
      position.x,
      screenRect.getPosition().y + screenRect.getSize().y * screenRect.getScale().y - 40.0f * screenRect.getScale().y));
  window.draw(backText);
}
