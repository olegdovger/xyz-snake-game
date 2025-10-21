#include "HighScores.hpp"
#include "../utils/FontInitializer.hpp"
#include "../utils/ScalingUtils.hpp"
#include "MainMenu.hpp"

using namespace shape;

HighScores::HighScores(sf::RenderWindow& win, Game& gameRef) : Screen(win, gameRef), titleText(font), backText(font) {
  font = FontInitializer::getDebugFont();

  screenRect.setSize(originSize);
  screenRect.setFillColor(menuBackgroundColor);
  screenRect.setOutlineColor(borderColor);
  screenRect.setOutlineThickness(10.0f);

  FontInitializer::initializeTitleText(titleText, font, L"Таблица рекордов");
  titleText.setLineSpacing(0.0f);
  FontInitializer::initializeBackText(backText, font, 24);

  game.loadSettings();
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

void HighScores::update() {}

void HighScores::render() {
  window.clear(backgroundColor);

  renderScreenRect();
  renderTitle();
  renderScores();
  renderBackButton();
}

void HighScores::renderScreenRect() {
  sf::Vector2u windowSize = window.getSize();

  screenRect.setPosition(sf::Vector2f((static_cast<float>(windowSize.x) - screenRect.getSize().x) / 2.0f,
                                      (static_cast<float>(windowSize.y) - screenRect.getSize().y) / 2.0f));

  const float scale = getScale(sf::Vector2f(screenRect.getSize()), window.getSize()) * 0.8f;
  screenRect.setScale(sf::Vector2f(scale, scale));

  const auto position = getPosition(sf::Vector2f(screenRect.getSize()), window.getSize(), scale);
  screenRect.setPosition(position);

  window.draw(screenRect);
}

void HighScores::renderTitle() {
  const auto position =
      getPosition(sf::Vector2f(titleText.getLocalBounds().size), window.getSize(), screenRect.getScale().x);
  titleText.setPosition(sf::Vector2f(position.x, screenRect.getPosition().y + 20 * screenRect.getScale().y));

  titleText.setScale(screenRect.getScale());

  window.draw(titleText);
}

void HighScores::renderScores() {
  const auto& recordTable = game.getSettingsReader().getGameRecordTable();

  const auto currentScore = game.getScore();

  for (size_t i = 0; i < recordTable.size(); ++i) {
    sf::Text item(font);

    item.setString(std::to_string(i + 1) + std::string(2, ' ') + std::string(14, '.') + std::string(2, ' ') +
                   std::to_string(recordTable[i]));

    const auto position =
        getPosition(sf::Vector2f(item.getLocalBounds().size), window.getSize(), screenRect.getScale().x);

    item.setPosition(sf::Vector2f(
        screenRect.getPosition().x + 120.0f * screenRect.getScale().x,
        screenRect.getPosition().y + 100.0f * screenRect.getScale().y + i * 50.0f * screenRect.getScale().y));

    item.setScale(screenRect.getScale());

    bool isLastOccurrence = false;
    if (currentScore == recordTable[i]) {
      isLastOccurrence = true;
      for (size_t j = i + 1; j < recordTable.size(); ++j) {
        if (currentScore == recordTable[j]) {
          isLastOccurrence = false;
          break;
        }
      }
    }

    if (isLastOccurrence) {
      item.setFillColor(sf::Color::Green);
    } else {
      item.setFillColor(sf::Color::White);
    }

    item.setStyle(sf::Text::Regular);

    window.draw(item);
  }

  if (recordTable.empty()) {
    sf::Text noScoresText(font);
    noScoresText.setString(L"Пока нет рекордов!");
    noScoresText.setCharacterSize(24);
    noScoresText.setFillColor(sf::Color::White);
    noScoresText.setStyle(sf::Text::Bold);

    sf::FloatRect textBounds = noScoresText.getLocalBounds();
    const auto position = getPosition(sf::Vector2f(textBounds.size), window.getSize(), screenRect.getScale().x);

    noScoresText.setPosition(position);
    window.draw(noScoresText);
  }
}

void HighScores::renderBackButton() {
  backText.setScale(screenRect.getScale());

  const auto position =
      getPosition(sf::Vector2f(backText.getLocalBounds().size), window.getSize(), screenRect.getScale().x);
  backText.setPosition(sf::Vector2f(
      position.x,
      screenRect.getPosition().y + screenRect.getSize().y * screenRect.getScale().y - 40.0f * screenRect.getScale().y));
  window.draw(backText);
}
