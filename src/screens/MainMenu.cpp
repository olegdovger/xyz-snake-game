#include <SFML/Graphics/Color.hpp>
#include <SFML/System/String.hpp>
#include <string>
#include <vector>
#include "../Game.hpp"
#include "../Screen.hpp"
#include "../utils/ResourceLoader.hpp"

#include "Difficulty.cpp"
#include "Exit.cpp"
#include "GameScreen.cpp"
#include "HighScores.cpp"
#include "Settings.cpp"

class MainMenu : public Screen {
private:
  sf::Font font;
  sf::Text titleText;
  std::vector<sf::Text> menuItems;
  std::vector<std::wstring> menuLabels;
  int selectedIndex = 0;

  sf::Color backgroundColor = sf::Color(164, 144, 164);

  // Colors
  static constexpr uint8_t BACKGROUND_R = 99;
  static constexpr uint8_t BACKGROUND_G = 155;
  static constexpr uint8_t BACKGROUND_B = 255;

  sf::Color menuBackgroundColor = sf::Color(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B);

  static constexpr uint8_t TEXT_R = 251;  // 251, 242, 54
  static constexpr uint8_t TEXT_G = 242;  // 251, 242, 54
  static constexpr uint8_t TEXT_B = 54;   // 251, 242, 54

  sf::Color textColor = sf::Color(TEXT_R, TEXT_G, TEXT_B);

  static constexpr uint8_t BORDER_R = 95;
  static constexpr uint8_t BORDER_G = 205;
  static constexpr uint8_t BORDER_B = 228;

  sf::Color borderColor = sf::Color(BORDER_R, BORDER_G, BORDER_B);

  // Draw main menu rectangle
  sf::RectangleShape menuRect;
  float scaleRelativeFactor = 0.8f;

  sf::Vector2f menuRectSize = sf::Vector2f(800.0f * scaleRelativeFactor, 600.0f * scaleRelativeFactor);

  void drawMenuBackground(sf::RenderWindow& window, const sf::Text& text) {
    sf::RectangleShape background;
    background.setSize(sf::Vector2f(text.getLocalBounds().size.x + 20, text.getLocalBounds().size.y + 10));
    background.setPosition(sf::Vector2f(text.getPosition().x - 10, text.getPosition().y - 5));
    background.setFillColor(sf::Color(BORDER_R, BORDER_G, BORDER_B));
    window.draw(background);
  }

public:
  explicit MainMenu(sf::RenderWindow& win, Game& gameRef) : Screen(win, gameRef), titleText(font) {}

  void processEvents(const sf::Event& event) override {
    if (event.is<sf::Event::KeyPressed>()) {
      switch (event.getIf<sf::Event::KeyPressed>()->code) {
        case sf::Keyboard::Key::W:
        case sf::Keyboard::Key::Up:
          std::cout << "Keypressed up(w)" << std::endl;
          selectedIndex = (selectedIndex - 1 + menuItems.size()) % menuItems.size();
          break;
        case sf::Keyboard::Key::S:
        case sf::Keyboard::Key::Down:
          std::cout << "Keypressed down(s)" << std::endl;
          selectedIndex = (selectedIndex + 1) % menuItems.size();
          break;
        case sf::Keyboard::Key::Enter:
          switch (selectedIndex) {
            case 0:  // Начать игру
              game.setCurrentScreen(new GameScreen(window, game));
              break;
            case 1:  // Уровень сложности
              game.setCurrentScreen(new Difficulty(window, game));
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
        case sf::Keyboard::Key::Escape:
          game.setCurrentScreen(new Exit(window, game));
          break;
        default:
          break;
      }
    }
  }

  void update() override {
    // No update logic needed for menu
  }

  void renderMenuRect() {
    menuRect.setSize(menuRectSize);

    sf::Vector2u windowSize = window.getSize();

    menuRect.setPosition(
        sf::Vector2f(windowSize.x / 2.0f - menuRectSize.x / 2.0f, windowSize.y / 2.0f - menuRectSize.y / 2.0f));

    menuRect.setFillColor(menuBackgroundColor);
    menuRect.setOutlineColor(borderColor);
    menuRect.setOutlineThickness(10.0f);

    window.draw(menuRect);
  }

  void renderTitle() {
    // Load font using ResourceLoader
    font = utils::ResourceLoader::getFont(utils::FontType::DebugFont);

    // Initialize title
    titleText.setFont(font);
    titleText.setString(L"Змейка");
    titleText.setCharacterSize(60);
    titleText.setLineSpacing(0.0f);
    titleText.setFillColor(textColor);
    titleText.setStyle(sf::Text::Bold);
    titleText.setOutlineColor(textColor - sf::Color(10, 10, 10));
    titleText.setOutlineThickness(2.0f);

    // Center title horizontally
    sf::Vector2u windowSize = window.getSize();
    sf::FloatRect titleBounds = titleText.getLocalBounds();

    sf::Vector2f centerPosition =
        sf::Vector2f(menuRect.getPosition().x + menuRect.getSize().x / 2.0f - titleBounds.size.x / 2.0f,
                     menuRect.getPosition().y + 20);

    titleText.setPosition(centerPosition);

    window.draw(titleText);
  }

  void renderMenuItems() {
    // Initialize menu items
    menuLabels = {L"Начать игру", L"Уровень сложности", L"Таблица рекордов", L"Настройки", L"Выход"};

    menuItems.clear();

    // Center title horizontally
    sf::Vector2u windowSize = window.getSize();

    for (size_t i = 0; i < menuLabels.size(); ++i) {
      sf::Text menuItem(font);

      menuItem.setCharacterSize(28);
      menuItem.setFillColor(sf::Color::White);
      menuItem.setString(menuLabels[i]);
      menuItem.setStyle(sf::Text::Bold);

      sf::Vector2f position =
          sf::Vector2f(menuRect.getPosition().x + 180.0f, menuRect.getPosition().y + 120.0f + i * 40.0f);

      menuItem.setPosition(position);

      if (i == selectedIndex) {
        menuItem.setFillColor(textColor);
      }

      window.draw(menuItem);
    }
  }

  void render() override {
    // std::cout << "MainMenu screen - Rendering menu" << std::endl;
    // Set background color (outside rectangle)
    window.clear(backgroundColor);

    renderMenuRect();
    renderTitle();

    renderMenuItems();
  }
};
