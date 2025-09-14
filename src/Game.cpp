#include "Game.hpp"
#include <SFML/Graphics.hpp>
// #include "screens/GameScreen.cpp"
#include "screens/MainMenu.cpp"
#include "utils/DebugUI.hpp"
#include "utils/EventLogger.hpp"
#include "utils/ResourceLoader.hpp"

Game::Game(sf::RenderWindow& win) : window(win), isRunning(true) {
  //initializeAllResources
  utils::ResourceLoader::initializeAllResources();

  setCurrentScreen(new MainMenu(window, *this));

  // Initialize debug UI
  utils::DebugUI::initialize(window);
  utils::EventLogger::setDebugMode(DEBUG_UI_TEXT);
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

    // Render debug UI if enabled
    if (DEBUG_UI_TEXT) {
      utils::DebugUI::render(window);
    }

    window.display();
  }
}

void Game::processEvents(const sf::Event& event) const {
  // Log event details using utility method
  utils::EventLogger::logEvent(event);

  // Handle specific events
  if (event.is<sf::Event::Closed>()) {
    std::cout << "Window closed" << std::endl;
    window.close();
  }

  // Игнорируем события ресайза для предотвращения подвисания
  if (event.is<sf::Event::Resized>()) {

    const auto resizedEvent = event.getIf<sf::Event::Resized>();
    // Update the view to match the new window size
    sf::View view = sf::View(sf::Vector2f(resizedEvent->size.x / 2.0f, resizedEvent->size.y / 2.0f),
                             sf::Vector2f(resizedEvent->size.x, resizedEvent->size.y));
    window.setView(view);
  }
}

void Game::setCurrentScreen(Screen* screen) {
  currentScreen = screen;
}
