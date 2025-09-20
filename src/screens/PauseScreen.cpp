#include "PauseScreen.hpp"
#include "../utils/ResourceLoader.hpp"
#include "Exit.hpp"
#include "GameScreen.hpp"
#include "MainMenu.hpp"

PauseScreen::PauseScreen(sf::RenderWindow& win, Game& gameRef) : Screen(win, gameRef), titleText(font) {}

void PauseScreen::drawMenuBackground(sf::RenderWindow& window, const sf::Text& text) {
  sf::RectangleShape background;
  background.setSize(sf::Vector2f(text.getLocalBounds().size.x + 20, text.getLocalBounds().size.y + 10));
  background.setPosition(sf::Vector2f(text.getPosition().x - 10, text.getPosition().y - 5));
  background.setFillColor(sf::Color(BORDER_R, BORDER_G, BORDER_B));
  window.draw(background);
}

void PauseScreen::processEvents(const sf::Event& event) {
  if (event.is<sf::Event::KeyPressed>()) {
    switch (event.getIf<sf::Event::KeyPressed>()->code) {
      case sf::Keyboard::Key::W:
      case sf::Keyboard::Key::Up:
        std::cout << "Keypressed up(w)" << std::endl;
        selectedIndex = (selectedIndex - 1 + MENU_ITEMS_COUNT) % MENU_ITEMS_COUNT;
        break;
      case sf::Keyboard::Key::S:
      case sf::Keyboard::Key::Down:
        std::cout << "Keypressed down(s)" << std::endl;
        selectedIndex = (selectedIndex + 1) % MENU_ITEMS_COUNT;
        break;
      case sf::Keyboard::Key::Enter:
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

void PauseScreen::renderMenuRect() {
  menuRect.setSize(menuRectSize);

  sf::Vector2u windowSize = window.getSize();

  menuRect.setPosition(
      sf::Vector2f(windowSize.x / 2.0f - menuRectSize.x / 2.0f, windowSize.y / 2.0f - menuRectSize.y / 2.0f));

  menuRect.setFillColor(menuBackgroundColor);
  menuRect.setOutlineColor(borderColor);
  menuRect.setOutlineThickness(10.0f);

  window.draw(menuRect);
}

void PauseScreen::renderTitle() {
  // Load font using ResourceLoader
  font = utils::ResourceLoader::getFont(utils::FontType::DebugFont);

  // Initialize title
  titleText.setFont(font);
  titleText.setString(L"Пауза");
  titleText.setCharacterSize(50);
  titleText.setLineSpacing(0.0f);
  titleText.setFillColor(sf::Color::White);
  titleText.setStyle(sf::Text::Bold);

  sf::FloatRect titleBounds = titleText.getLocalBounds();

  sf::Vector2f centerPosition =
      sf::Vector2f(menuRect.getPosition().x + menuRect.getSize().x / 2.0f - titleBounds.size.x / 2.0f,
                   menuRect.getPosition().y + 20);

  titleText.setPosition(centerPosition);

  window.draw(titleText);
}

void PauseScreen::renderMenuItems() {
  // Initialize menu items
  menuLabels = {L"Продолжить игру", L"Главное меню", L"Выход"};

  menuItems.clear();

  for (size_t i = 0; i < menuLabels.size(); ++i) {
    sf::Text menuItem(font);

    menuItem.setCharacterSize(24);
    menuItem.setFillColor(sf::Color::White);
    menuItem.setString(menuLabels[i]);
    menuItem.setStyle(sf::Text::Bold);

    sf::Vector2f position =
        sf::Vector2f(menuRect.getPosition().x + 120.0f, menuRect.getPosition().y + 140.0f + i * 40.0f);

    menuItem.setPosition(position);

    if (i == selectedIndex) {
      menuItem.setFillColor(textColor);
      menuItem.setStyle(sf::Text::Bold | sf::Text::Underlined);
    }

    window.draw(menuItem);
  }
}

void PauseScreen::render() {
  window.clear(backgroundColor);

  renderMenuRect();
  renderTitle();
  renderMenuItems();
}
