#include "DifficultyScreen.hpp"
#include <iostream>
#include "../utils/ResourceLoader.hpp"
#include "../utils/ScalingUtils.hpp"
#include "../utils/SettingStorage.hpp"
#include "GameScreen.hpp"
#include "MainMenu.hpp"

using namespace shape;

DifficultyScreen::DifficultyScreen(sf::RenderWindow& win, Game& gameRef)
    : Screen(win, gameRef), titleText(font), backText(font) {
  // Initialize screen rectangle
  screenRect.setSize(originSize);
  screenRect.setFillColor(menuBackgroundColor);
  screenRect.setOutlineColor(borderColor);
  screenRect.setOutlineThickness(10.0f);

  // Initialize title
  font = ResourceLoader::getFont(FontType::DebugFont);

  titleText.setFont(font);
  titleText.setString(L"Уровень сложности");
  titleText.setCharacterSize(40);
  titleText.setFillColor(sf::Color::White);
  titleText.setStyle(sf::Text::Bold);

  backText.setFont(font);
  backText.setString(L"Назад (Escape)");
  backText.setCharacterSize(24);
  backText.setFillColor(sf::Color::White);
  backText.setStyle(sf::Text::Bold);

  // Initialize difficulty items
  initializeDifficultyItems();
}

void DifficultyScreen::processEvents(const sf::Event& event) {
  if (event.is<sf::Event::KeyPressed>()) {
    switch (event.getIf<sf::Event::KeyPressed>()->code) {
      case sf::Keyboard::Key::Up:
        selectPreviousDifficulty();
        break;
      case sf::Keyboard::Key::Down:
        selectNextDifficulty();
        break;
      case sf::Keyboard::Key::Enter:
      case sf::Keyboard::Key::Space:
        confirmSelection();
        break;
      case sf::Keyboard::Key::Escape:
        game.setCurrentScreen(new MainMenu(window, game));
        break;
      default:
        break;
    }
  }
}

void DifficultyScreen::update() {}

void DifficultyScreen::render() {
  renderScreenRect();
  renderTitle();
  renderScreenItems();
  renderBackButton();
}

void DifficultyScreen::renderScreenRect() {
  // Center the rectangle
  sf::Vector2u windowSize = window.getSize();
  screenRect.setPosition(sf::Vector2f((static_cast<float>(windowSize.x) - screenRect.getSize().x) / 2.0f,
                                      (static_cast<float>(windowSize.y) - screenRect.getSize().y) / 2.0f));

  const float scale = getScale(sf::Vector2f(screenRect.getSize()), window.getSize()) * 0.8f;

  screenRect.setScale(sf::Vector2f(scale, scale));
  const auto position = getPosition(sf::Vector2f(screenRect.getSize()), window.getSize(), scale);

  screenRect.setPosition(position);

  window.draw(screenRect);
}

void DifficultyScreen::renderTitle() {
  const auto position =
      getPosition(sf::Vector2f(titleText.getLocalBounds().size), window.getSize(), screenRect.getScale().x);
  titleText.setPosition(sf::Vector2f(position.x, screenRect.getPosition().y + 20 * screenRect.getScale().y));

  titleText.setScale(screenRect.getScale());

  window.draw(titleText);
}

void DifficultyScreen::renderScreenItems() {

  for (size_t i = 0; i < difficultyItems.size(); ++i) {

    const auto position =
        getPosition(sf::Vector2f(difficultyItems[i].getLocalBounds().size), window.getSize(), screenRect.getScale().x);

    difficultyItems[i].setPosition(sf::Vector2f(
        position.x,
        screenRect.getPosition().y + 100.0f * screenRect.getScale().y + i * 50.0f * screenRect.getScale().y));

    difficultyItems[i].setScale(screenRect.getScale());
    window.draw(difficultyItems[i]);

    if (i == selectedDifficultyIndex) {
      difficultyItems[i].setFillColor(textColor);
      difficultyItems[i].setStyle(sf::Text::Underlined);
    } else {
      difficultyItems[i].setFillColor(sf::Color::White);
      difficultyItems[i].setStyle(sf::Text::Regular);
    }
  }
}

void DifficultyScreen::renderBackButton() {
  backText.setScale(screenRect.getScale());

  const auto position =
      getPosition(sf::Vector2f(backText.getLocalBounds().size), window.getSize(), screenRect.getScale().x);
  backText.setPosition(sf::Vector2f(
      position.x,
      screenRect.getPosition().y + screenRect.getSize().y * screenRect.getScale().y - 40.0f * screenRect.getScale().y));
  window.draw(backText);
}

void DifficultyScreen::initializeDifficultyItems() {
  std::vector<std::wstring> textItems = {L"Легкий", L"Труднее, чем легкий", L"Средний", L"Труднее, чем средний",
                                         L"Сложный"};

  difficultyItems.clear();
  difficultyItems.reserve(textItems.size());

  for (size_t i = 0; i < textItems.size(); ++i) {
    sf::Text text(font);
    text.setString(textItems[i]);
    text.setCharacterSize(32);
    text.setFillColor(sf::Color::White);
    difficultyItems.push_back(text);
  }
}

void DifficultyScreen::selectNextDifficulty() {
  selectedDifficultyIndex = (selectedDifficultyIndex + 1) % difficultyItems.size();
}

void DifficultyScreen::selectPreviousDifficulty() {
  selectedDifficultyIndex = (selectedDifficultyIndex - 1 + difficultyItems.size()) % difficultyItems.size();
}

void DifficultyScreen::confirmSelection() {
  std::vector<GameLevel> levels = {GameLevel::Easy, GameLevel::HarderThanEasy, GameLevel::Middle,
                                   GameLevel::HarderThanMiddle, GameLevel::Hard};

  if (selectedDifficultyIndex < levels.size()) {
    game.setGameLevel(levels[selectedDifficultyIndex]);
    std::cout << "Selected difficulty: " << getDifficultyDisplayName(levels[selectedDifficultyIndex]) << std::endl;

    // Save settings to file
    SettingStorage settingStorage;
    settingStorage.setGameLevel(levels[selectedDifficultyIndex]);
    if (settingStorage.saveSettings()) {
      std::cout << "Difficulty saved to settings" << std::endl;
    } else {
      std::cerr << "Failed to save difficulty to settings" << std::endl;
    }

    // Navigate to game screen
    game.setCurrentScreen(new GameScreen(window, game));
  }
}

std::string DifficultyScreen::getDifficultyDisplayName(GameLevel level) {
  switch (level) {
    case GameLevel::Easy:
      return "Easy";
    case GameLevel::HarderThanEasy:
      return "Harder Than Easy";
    case GameLevel::Middle:
      return "Middle";
    case GameLevel::HarderThanMiddle:
      return "Harder Than Middle";
    case GameLevel::Hard:
      return "Hard";
    default:
      return "Unknown";
  }
}
