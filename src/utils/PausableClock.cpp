#include "PausableClock.hpp"
#include <chrono>

PausableClock::PausableClock() : pausedTime(0.0f), isPausedLocally(false) {
  startTime = std::chrono::steady_clock::now();
  TimerManager::getInstance().registerTimer(this);
}

PausableClock::~PausableClock() {
  TimerManager::getInstance().unregisterTimer(this);
}

float PausableClock::getElapsedTime() const {
  if (isPausedLocally) {
    return pausedTime;
  }
  auto now = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
  return (duration.count() / 1000.0f) - pausedTime;
}

void PausableClock::restart() {
  startTime = std::chrono::steady_clock::now();
  pausedTime = 0.0f;
  isPausedLocally = false;
}

void PausableClock::pause() {
  if (!isPausedLocally) {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
    pausedTime += duration.count() / 1000.0f;
    isPausedLocally = true;
  }
}

void PausableClock::unpause() {
  if (isPausedLocally) {
    startTime = std::chrono::steady_clock::now();
    isPausedLocally = false;
  }
}
