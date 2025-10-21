#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Game.hpp"
#include "screens/GameScreen.hpp"
#include "screens/MainMenu.hpp"
#include "utils/DebugUI.hpp"
#include "utils/EventLogger.hpp"

Game::Game(sf::RenderWindow& win) : window(win), isRunning(true), previousScreen(nullptr) {
  if (!settingStorage.loadSettings()) {
    std::cerr << "Warning: Failed to load settings, using defaults" << std::endl;
  }

  setCurrentScreen(new MainMenu(window, *this));

  DebugUI::initialize(window);
  EventLogger::setDebugMode(DEBUG_UI_TEXT);
}

Game::~Game() {
  delete currentScreen;
  delete previousScreen;
}

void Game::start() const {
  while (window.isOpen()) {
    sf::sleep(sf::milliseconds(16));

    window.clear(sf::Color(164, 144, 164));

    while (const auto event = window.pollEvent()) {
      processEvents(*event);
      currentScreen->processEvents(*event);
    }

    currentScreen->update();
    currentScreen->render();

    if (DEBUG_UI_TEXT) {
      DebugUI::render(window);
    }

    window.display();
  }
}

void Game::processEvents(const sf::Event& event) const {
  EventLogger::logEvent(event);

  if (event.is<sf::Event::Closed>()) {
    std::cout << "Window closed" << std::endl;
    window.close();
  }

  if (event.is<sf::Event::Resized>()) {

    const auto resizedEvent = event.getIf<sf::Event::Resized>();
    sf::View view = sf::View(sf::Vector2f(resizedEvent->size.x / 2.0f, resizedEvent->size.y / 2.0f),
                             sf::Vector2f(resizedEvent->size.x, resizedEvent->size.y));
    window.setView(view);
  }
}

void Game::setCurrentScreen(Screen* screen) {
  delete currentScreen;
  currentScreen = screen;
}

void Game::setCurrentScreenWithPrevious(Screen* screen, Screen* previous) {
  delete previousScreen;
  previousScreen = previous;
  currentScreen = screen;
}

void Game::returnToPreviousScreen() {
  if (previousScreen != nullptr) {
    delete currentScreen;
    currentScreen = previousScreen;
    previousScreen = nullptr;
  }
}

void Game::returnToGameScreen() {
  if (previousScreen != nullptr) {
    if (auto* gameScreen = dynamic_cast<GameScreen*>(previousScreen)) {
      gameScreen->resume();
    }
    delete currentScreen;
    currentScreen = previousScreen;
    previousScreen = nullptr;
  } else {
    delete currentScreen;
    currentScreen = new GameScreen(window, *this);
  }
}

void Game::saveScoreToRecordTable() {

  settingStorage.loadSettings();
  settingStorage.addScoreToRecordTable(score);
}

bool Game::isNewHighScore() const {
  const auto& recordTable = settingStorage.getGameRecordTable();
  return recordTable.empty() || score > recordTable.back();
}
