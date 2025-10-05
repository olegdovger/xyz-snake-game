#include "EventLogger.hpp"
#include "DebugUI.hpp"

bool EventLogger::debugMode = false;
namespace {
std::string getKeyName(const sf::Keyboard::Key key) {
  switch (key) {
    case sf::Keyboard::Key::A:
      return "A";
    case sf::Keyboard::Key::B:
      return "B";
    case sf::Keyboard::Key::C:
      return "C";
    case sf::Keyboard::Key::D:
      return "D";
    case sf::Keyboard::Key::E:
      return "E";
    case sf::Keyboard::Key::F:
      return "F";
    case sf::Keyboard::Key::G:
      return "G";
    case sf::Keyboard::Key::H:
      return "H";
    case sf::Keyboard::Key::I:
      return "I";
    case sf::Keyboard::Key::J:
      return "J";
    case sf::Keyboard::Key::K:
      return "K";
    case sf::Keyboard::Key::L:
      return "L";
    case sf::Keyboard::Key::M:
      return "M";
    case sf::Keyboard::Key::N:
      return "N";
    case sf::Keyboard::Key::O:
      return "O";
    case sf::Keyboard::Key::P:
      return "P";
    case sf::Keyboard::Key::Q:
      return "Q";
    case sf::Keyboard::Key::R:
      return "R";
    case sf::Keyboard::Key::S:
      return "S";
    case sf::Keyboard::Key::T:
      return "T";
    case sf::Keyboard::Key::U:
      return "U";
    case sf::Keyboard::Key::V:
      return "V";
    case sf::Keyboard::Key::W:
      return "W";
    case sf::Keyboard::Key::X:
      return "X";
    case sf::Keyboard::Key::Y:
      return "Y";
    case sf::Keyboard::Key::Z:
      return "Z";
    case sf::Keyboard::Key::Num0:
      return "0";
    case sf::Keyboard::Key::Num1:
      return "1";
    case sf::Keyboard::Key::Num2:
      return "2";
    case sf::Keyboard::Key::Num3:
      return "3";
    case sf::Keyboard::Key::Num4:
      return "4";
    case sf::Keyboard::Key::Num5:
      return "5";
    case sf::Keyboard::Key::Num6:
      return "6";
    case sf::Keyboard::Key::Num7:
      return "7";
    case sf::Keyboard::Key::Num8:
      return "8";
    case sf::Keyboard::Key::Num9:
      return "9";
    case sf::Keyboard::Key::Escape:
      return "Escape";
    case sf::Keyboard::Key::Space:
      return "Space";
    case sf::Keyboard::Key::Enter:
      return "Enter";
    case sf::Keyboard::Key::Backspace:
      return "Backspace";
    case sf::Keyboard::Key::Tab:
      return "Tab";
    case sf::Keyboard::Key::Left:
      return "Left";
    case sf::Keyboard::Key::Right:
      return "Right";
    case sf::Keyboard::Key::Up:
      return "Up";
    case sf::Keyboard::Key::Down:
      return "Down";
    case sf::Keyboard::Key::LControl:
    case sf::Keyboard::Key::RControl:
      return "Ctrl";
    case sf::Keyboard::Key::LAlt:
    case sf::Keyboard::Key::RAlt:
      return "Alt";
    case sf::Keyboard::Key::LShift:
    case sf::Keyboard::Key::RShift:
      return "Shift";
    case sf::Keyboard::Key::LSystem:
    case sf::Keyboard::Key::RSystem:
      return "Win";
    default:
      return "Unknown(" + std::to_string(static_cast<int>(key)) + ")";
  }
}

std::string getModifiers() {
  std::string result;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl)) {
    result += "Ctrl+";
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RAlt)) {
    result += "Alt+";
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)) {
    result += "Shift+";
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RSystem)) {
    result += "Win+";
  }

  return result;
}

bool isModifierKey(const sf::Keyboard::Key key) {
  return key == sf::Keyboard::Key::LControl || key == sf::Keyboard::Key::RControl || key == sf::Keyboard::Key::LAlt ||
         key == sf::Keyboard::Key::RAlt || key == sf::Keyboard::Key::LShift || key == sf::Keyboard::Key::RShift ||
         key == sf::Keyboard::Key::LSystem || key == sf::Keyboard::Key::RSystem;
}
}  // namespace

void EventLogger::setDebugMode(const bool enabled) {
  debugMode = enabled;
}

void EventLogger::logEvent(const sf::Event& event) {
  std::string eventText;

  if (event.is<sf::Event::KeyPressed>()) {
    const auto keyEvent = event.getIf<sf::Event::KeyPressed>();
    if (isModifierKey(keyEvent->code)) {
      eventText = "KeyPressed: " + getKeyName(keyEvent->code);
    } else {
      std::string modifiers = getModifiers();
      eventText = "KeyPressed: " + modifiers + getKeyName(keyEvent->code);
    }
  } else if (event.is<sf::Event::KeyReleased>()) {
    const auto keyEvent = event.getIf<sf::Event::KeyReleased>();
    if (isModifierKey(keyEvent->code)) {
      eventText = "KeyReleased: " + getKeyName(keyEvent->code);
    } else {
      std::string modifiers = getModifiers();
      eventText = "KeyReleased: " + modifiers + getKeyName(keyEvent->code);
    }
  } else if (event.is<sf::Event::MouseButtonPressed>()) {
    const auto mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
    std::string buttonName;
    switch (mouseEvent->button) {
      case sf::Mouse::Button::Left:
        buttonName = "Left";
        break;
      case sf::Mouse::Button::Right:
        buttonName = "Right";
        break;
      case sf::Mouse::Button::Middle:
        buttonName = "Middle";
        break;
      default:
        buttonName = "Other";
        break;
    }
    eventText = "MousePressed: " + buttonName + " at (" + std::to_string(mouseEvent->position.x) + ", " +
                std::to_string(mouseEvent->position.y) + ")";
  } else if (event.is<sf::Event::MouseButtonReleased>()) {
    const auto mouseEvent = event.getIf<sf::Event::MouseButtonReleased>();
    std::string buttonName;
    switch (mouseEvent->button) {
      case sf::Mouse::Button::Left:
        buttonName = "Left";
        break;
      case sf::Mouse::Button::Right:
        buttonName = "Right";
        break;
      case sf::Mouse::Button::Middle:
        buttonName = "Middle";
        break;
      default:
        buttonName = "Other";
        break;
    }
    eventText = "MouseReleased: " + buttonName + " at (" + std::to_string(mouseEvent->position.x) + ", " +
                std::to_string(mouseEvent->position.y) + ")";
  } else if (event.is<sf::Event::MouseMoved>()) {
    const auto mouseEvent = event.getIf<sf::Event::MouseMoved>();
    eventText =
        "MouseMoved: (" + std::to_string(mouseEvent->position.x) + ", " + std::to_string(mouseEvent->position.y) + ")";
  } else if (event.is<sf::Event::Closed>()) {
    eventText = "WindowClosed";
  } else if (event.is<sf::Event::Resized>()) {
    const auto resizeEvent = event.getIf<sf::Event::Resized>();
    eventText = "WindowResized: " + std::to_string(resizeEvent->size.x) + "x" + std::to_string(resizeEvent->size.y);
  } else if (event.is<sf::Event::TextEntered>()) {
    const auto textEvent = event.getIf<sf::Event::TextEntered>();
    eventText = "TextEntered: " + std::string(1, static_cast<char>(textEvent->unicode));
  } else {
    eventText = "Other Event";
  }

  if (debugMode) {
    DebugUI::addDebugText(eventText);
  } else {
    std::cout << "Event: " << eventText << std::endl;
  }
}
