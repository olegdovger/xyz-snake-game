#include "TimerManager.hpp"
#include <algorithm>

TimerManager& TimerManager::getInstance() {
  static TimerManager instance;
  return instance;
}

void TimerManager::registerTimer(PausableTimer* timer) {
  if (timer && std::find(timers.begin(), timers.end(), timer) == timers.end()) {
    timers.push_back(timer);
  }
}

void TimerManager::unregisterTimer(PausableTimer* timer) {
  timers.erase(std::remove(timers.begin(), timers.end(), timer), timers.end());
}

void TimerManager::pauseAll() {
  paused = true;
  for (auto* timer : timers) {
    if (timer) {
      timer->pause();
    }
  }
}

void TimerManager::unpauseAll() {
  paused = false;
  for (auto* timer : timers) {
    if (timer) {
      timer->unpause();
    }
  }
}
