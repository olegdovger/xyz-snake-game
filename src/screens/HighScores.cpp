#include "HighScores.hpp"
#include "../utils/ResourceLoader.hpp"
#include "MainMenu.hpp"

HighScores::HighScores(sf::RenderWindow& win, Game& gameRef) : Screen(win, gameRef), titleText(font), backText(font) {
  // Load font
  font = utils::ResourceLoader::getFont(utils::FontType::DebugFont);

  // Initialize title
  titleText.setFont(font);
  titleText.setString(L"Таблица рекордов");
  titleText.setCharacterSize(40);
  titleText.setFillColor(sf::Color::White);
  titleText.setStyle(sf::Text::Bold);

  // Initialize back button
  backText.setFont(font);
  backText.setString(L"Назад (ESC)");
  backText.setCharacterSize(24);
  backText.setFillColor(sf::Color::White);
  backText.setStyle(sf::Text::Bold);
}

void HighScores::processEvents(const sf::Event& event) {
  if (event.is<sf::Event::KeyPressed>()) {
    switch (event.getIf<sf::Event::KeyPressed>()->code) {
      case sf::Keyboard::Key::Escape:
        game.setCurrentScreen(new MainMenu(window, game));
        break;
      default:
        break;
    }
  }
}

void HighScores::update() {
  // No update logic needed
}

void HighScores::render() {
  window.clear(backgroundColor);

  renderMenuRect();
  renderTitle();
  renderScores();
  renderBackButton();
}

void HighScores::renderMenuRect() {
  menuRect.setSize(menuRectSize);

  sf::Vector2u windowSize = window.getSize();
  menuRect.setPosition(
      sf::Vector2f(windowSize.x / 2.0f - menuRectSize.x / 2.0f, windowSize.y / 2.0f - menuRectSize.y / 2.0f));

  menuRect.setFillColor(menuBackgroundColor);
  menuRect.setOutlineColor(borderColor);
  menuRect.setOutlineThickness(8.0f);

  window.draw(menuRect);
}

void HighScores::renderTitle() {
  sf::Vector2u windowSize = window.getSize();
  sf::FloatRect titleBounds = titleText.getLocalBounds();

  sf::Vector2f centerPosition =
      sf::Vector2f(menuRect.getPosition().x + menuRect.getSize().x / 2.0f - titleBounds.size.x / 2.0f,
                   menuRect.getPosition().y + 20);
  titleText.setPosition(centerPosition);
  window.draw(titleText);
}

void HighScores::renderScores() {
  const auto& recordTable = game.getSettingsReader().getGameRecordTable();

  scoreTexts.clear();

  for (size_t i = 0; i < recordTable.size() && i < 5; ++i) {
    sf::Text scoreText(font);
    scoreText.setCharacterSize(28);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setStyle(sf::Text::Bold);

    // Format: "1. 100"
    std::string scoreString = std::to_string(i + 1) + ". " + std::to_string(recordTable[i]);
    scoreText.setString(scoreString);

    // Position scores vertically
    sf::Vector2f position =
        sf::Vector2f(menuRect.getPosition().x + 50.0f, menuRect.getPosition().y + 120.0f + i * 50.0f);

    scoreText.setPosition(position);
    scoreTexts.push_back(scoreText);
    window.draw(scoreText);
  }

  // If no scores, show message
  if (recordTable.empty()) {
    sf::Text noScoresText(font);
    noScoresText.setString(L"Пока нет рекордов!");
    noScoresText.setCharacterSize(24);
    noScoresText.setFillColor(sf::Color::White);
    noScoresText.setStyle(sf::Text::Bold);

    sf::FloatRect textBounds = noScoresText.getLocalBounds();
    sf::Vector2f position = sf::Vector2f(menuRect.getPosition().x + menuRectSize.x / 2.0f - textBounds.size.x / 2.0f,
                                         menuRect.getPosition().y + menuRectSize.y / 2.0f);

    noScoresText.setPosition(position);
    window.draw(noScoresText);
  }
}

void HighScores::renderBackButton() {
  sf::FloatRect textBounds = backText.getLocalBounds();

  sf::Vector2f centerPosition =
      sf::Vector2f(menuRect.getPosition().x + menuRect.getSize().x / 2.0f - textBounds.size.x / 2.0f,
                   menuRect.getPosition().y + menuRect.getSize().y - 40);

  backText.setPosition(centerPosition);
  window.draw(backText);
}
