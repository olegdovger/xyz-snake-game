#include "CountdownTimer.hpp"
#include "../config/AudioConstants.hpp"
#include "ResourceLoader.hpp"

CountdownTimer::CountdownTimer(int totalSeconds, bool soundEnabled)
    : totalSeconds(totalSeconds),
      currentSeconds(totalSeconds),
      isActive(false),
      isFinished(false),
      soundEnabled(soundEnabled),
      countdownText(ResourceLoader::getFont(FontType::DebugFont)),
      countdownSound(ResourceLoader::getSound(SoundType::Countdown)) {

  countdownSound.setVolume(AudioConstants::SoundEffects::COUNTDOWN_VOLUME);

  countdownText.setCharacterSize(72.0f);
  countdownText.setFillColor(sf::Color(130, 73, 113, 255));
  countdownText.setStyle(sf::Text::Bold);
  countdownText.setOutlineColor(sf::Color::White);
  countdownText.setOutlineThickness(3.0f);

  updateText();
}

void CountdownTimer::start() {
  isActive = true;
  isFinished = false;
  currentSeconds = totalSeconds;
  clock.restart();
  updateText();

  if (soundEnabled && currentSeconds > 0) {
    countdownSound.play();
  }
}

void CountdownTimer::update() {
  if (!isActive || isFinished) {
    return;
  }

  float elapsed = clock.getElapsedTime().asSeconds();
  int newCurrentSeconds = totalSeconds - static_cast<int>(elapsed);

  if (newCurrentSeconds != currentSeconds) {
    currentSeconds = newCurrentSeconds;
    updateText();

    if (soundEnabled && currentSeconds > 0) {
      countdownSound.play();
    }
  }

  if (currentSeconds <= 0) {
    isActive = false;
    isFinished = true;
    currentSeconds = 0;
    updateText();
  }
}

void CountdownTimer::render(sf::RenderWindow& window) {
  if (isActive) {
    window.draw(countdownText);
  }
}

void CountdownTimer::reset() {
  isActive = false;
  isFinished = false;
  currentSeconds = totalSeconds;
  updateText();
}

void CountdownTimer::setDuration(int seconds) {
  totalSeconds = seconds;
  currentSeconds = seconds;
  updateText();
}

void CountdownTimer::setPosition(const sf::Vector2f& position) {
  countdownText.setPosition(position);
}

void CountdownTimer::setSoundEnabled(bool enabled) {
  soundEnabled = enabled;
}

void CountdownTimer::updateText() {
  if (currentSeconds > 0) {
    countdownText.setString(std::to_string(currentSeconds));
  } else {
    countdownText.setString("GO!");
  }

  sf::FloatRect textRect = countdownText.getLocalBounds();
  countdownText.setOrigin(
      sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f));
}
