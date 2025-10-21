#include "MainMenu.hpp"
#include <iostream>
#include "../utils/FontInitializer.hpp"
#include "../utils/ScalingUtils.hpp"
#include "../utils/SettingStorage.hpp"
#include "DifficultyScreen.hpp"
#include "Exit.hpp"
#include "GameScreen.hpp"
#include "HighScores.hpp"
#include "Settings.hpp"

using namespace shape;

MainMenu::MainMenu(sf::RenderWindow& win, Game& gameRef) : Screen(win, gameRef), titleText(font) {
  font = FontInitializer::getDebugFont();
  FontInitializer::initializeTitleText(titleText, font, L"Главное меню");

  screenRect.setSize(originSize);
  screenRect.setFillColor(menuBackgroundColor);
  screenRect.setOutlineColor(borderColor);
  screenRect.setOutlineThickness(10.0f);

  screenRect.setSize(originSize);
  screenRect.setFillColor(menuBackgroundColor);
  screenRect.setOutlineColor(borderColor);
  screenRect.setOutlineThickness(10.0f);

  game.loadSettings();
  soundManager.setSoundEnabled(game.getSettingsReader().getGameSound());

  initializeMenuItems();
}

void MainMenu::drawMenuBackground(sf::RenderWindow& window, const sf::Text& text) const {
  sf::RectangleShape background;
  background.setSize(sf::Vector2f(text.getLocalBounds().size.x + 20, text.getLocalBounds().size.y + 10));
  background.setPosition(sf::Vector2f(text.getPosition().x - 10, text.getPosition().y - 5));
  background.setFillColor(sf::Color(MenuColors::BORDER_R, MenuColors::BORDER_G, MenuColors::BORDER_B));
  window.draw(background);
}

void MainMenu::processEvents(const sf::Event& event) {
  if (event.is<sf::Event::KeyPressed>()) {
    switch (event.getIf<sf::Event::KeyPressed>()->code) {
      case sf::Keyboard::Key::W:
      case sf::Keyboard::Key::Up:
        std::cout << "Keypressed up(w)" << std::endl;
        selectedIndex = (selectedIndex - 1 + MENU_ITEMS_COUNT) % MENU_ITEMS_COUNT;
        soundManager.playNavigationSound();  // Play sound when switching menu items
        break;
      case sf::Keyboard::Key::S:
      case sf::Keyboard::Key::Down:
        std::cout << "Keypressed down(s)" << std::endl;
        selectedIndex = (selectedIndex + 1) % MENU_ITEMS_COUNT;
        soundManager.playNavigationSound();  // Play sound when switching menu items
        break;
      case sf::Keyboard::Key::Enter:
        soundManager.playSelectionSound();  // Play sound when selecting menu item
        switch (selectedIndex) {
          case 0:  // Начать игру
            game.setCurrentScreen(new GameScreen(window, game));
            break;
          case 1:  // Уровень сложности
            game.setCurrentScreen(new DifficultyScreen(window, game));
            break;
          case 2:  // Таблица рекордов
            game.setCurrentScreen(new HighScores(window, game));
            break;
          case 3:  // Настройки
            game.setCurrentScreen(new Settings(window, game));
            break;
          case 4:  // Выход
            game.setCurrentScreen(new Exit(window, game));
            break;
        }
        break;
      default:
        break;
    }
  }
}

void MainMenu::update() {
  // No update logic needed for menu
}

void MainMenu::render() {
  renderMenuRect();
  renderTitle();
  renderMenuItems();
}

void MainMenu::initializeMenuItems() {
  menuLabels = {L"Начать игру", L"Уровень сложности", L"Таблица рекордов", L"Настройки", L"Выход"};

  menuItems.clear();
  menuItems.reserve(menuLabels.size());

  for (size_t i = 0; i < menuLabels.size(); ++i) {
    sf::Text item(font);
    FontInitializer::initializeMenuItemText(item, font, menuLabels[i], 24);
    menuItems.push_back(item);
  }
}

void MainMenu::renderMenuRect() {
  sf::Vector2u windowSize = window.getSize();
  screenRect.setPosition(sf::Vector2f((static_cast<float>(windowSize.x) - screenRect.getSize().x) / 2.0f,
                                      (static_cast<float>(windowSize.y) - screenRect.getSize().y) / 2.0f));

  const float scale = getScale(sf::Vector2f(screenRect.getSize()), window.getSize()) * 0.8f;
  screenRect.setScale(sf::Vector2f(scale, scale));

  const auto position = getPosition(sf::Vector2f(screenRect.getSize()), window.getSize(), scale);
  screenRect.setPosition(position);

  window.draw(screenRect);
}

void MainMenu::renderTitle() {
  const auto position =
      getPosition(sf::Vector2f(titleText.getLocalBounds().size), window.getSize(), screenRect.getScale().x);
  titleText.setPosition(sf::Vector2f(position.x, screenRect.getPosition().y + 20 * screenRect.getScale().y));

  titleText.setScale(screenRect.getScale());

  window.draw(titleText);
}

void MainMenu::renderMenuItems() {
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
