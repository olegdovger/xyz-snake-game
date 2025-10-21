#include "DifficultyScreen.hpp"

#include "../utils/FontInitializer.hpp"
#include "../utils/ScalingUtils.hpp"
#include "../utils/SettingStorage.hpp"
#include "../utils/difficulty/DifficultyManager.hpp"
#include "MainMenu.hpp"

using namespace shape;

DifficultyScreen::DifficultyScreen(sf::RenderWindow& win, Game& gameRef)
    : Screen(win, gameRef), titleText(font), backText(font) {

  screenRect.setSize(originSize);
  screenRect.setFillColor(menuBackgroundColor);
  screenRect.setOutlineColor(borderColor);
  screenRect.setOutlineThickness(10.0f);

  font = FontInitializer::getDebugFont();
  FontInitializer::initializeTitleText(titleText, font, L"Уровень сложности");
  FontInitializer::initializeBackText(backText, font, 24);

  initializeDifficultyItems();

  game.loadSettings();
  soundManager.setSoundEnabled(game.getSettingsReader().getGameSound());
}

void DifficultyScreen::processEvents(const sf::Event& event) {
  if (event.is<sf::Event::KeyPressed>()) {
    switch (event.getIf<sf::Event::KeyPressed>()->code) {
      case sf::Keyboard::Key::W:
      case sf::Keyboard::Key::Up:
        selectPreviousDifficulty();

        soundManager.playNavigationSound();

        break;
      case sf::Keyboard::Key::S:
      case sf::Keyboard::Key::Down:
        selectNextDifficulty();

        soundManager.playNavigationSound();

        break;
      case sf::Keyboard::Key::Enter:
      case sf::Keyboard::Key::Space:
        soundManager.playSelectionSound();

        confirmSelection();
        game.setCurrentScreen(new MainMenu(window, game));
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
  std::vector<GameDifficultyLevel> difficultyLevels = {
      GameDifficultyLevel::Easy, GameDifficultyLevel::HarderThanEasy, GameDifficultyLevel::Middle,
      GameDifficultyLevel::HarderThanMiddle, GameDifficultyLevel::Hard};

  difficultyItems.clear();
  difficultyItems.reserve(difficultyLevels.size());

  // Load current difficulty from settings file
  SettingStorage settingStorage;
  bool settingsLoaded = settingStorage.loadSettings();

  if (settingsLoaded) {
    GameDifficultyLevel currentDifficultyLevel = settingStorage.getGameDifficultyLevel();

    // Find and set the selected difficulty index
    for (size_t i = 0; i < difficultyLevels.size(); ++i) {
      if (difficultyLevels[i] == currentDifficultyLevel) {
        selectedDifficultyIndex = static_cast<int>(i);
        break;
      }
    }

  } else {
    // Failed to load settings, using default difficulty
  }

  for (size_t i = 0; i < difficultyLevels.size(); ++i) {
    sf::Text text(font);
    text.setString(DifficultyManager::getDifficultyDisplayName(difficultyLevels[i]));
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
  std::vector<GameDifficultyLevel> difficultyLevels = {
      GameDifficultyLevel::Easy, GameDifficultyLevel::HarderThanEasy, GameDifficultyLevel::Middle,
      GameDifficultyLevel::HarderThanMiddle, GameDifficultyLevel::Hard};

  if (selectedDifficultyIndex < difficultyLevels.size()) {
    // Use the existing SettingStorage from Game instead of creating a new one
    SettingStorage& settingStorage = const_cast<SettingStorage&>(game.getSettingsReader());
    settingStorage.setGameDifficultyLevel(difficultyLevels[selectedDifficultyIndex]);
    settingStorage.saveSettings();
  }
}
