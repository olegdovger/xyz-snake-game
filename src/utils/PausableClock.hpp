#pragma once
#include <chrono>
#include "TimerManager.hpp"

class PausableClock : public TimerManager::PausableTimer {
public:
  PausableClock();
  ~PausableClock();

  float getElapsedTime() const;
  void restart();
  void pause() override;
  void unpause() override;

private:
  std::chrono::steady_clock::time_point startTime;
  float pausedTime;
  bool isPausedLocally;
};
